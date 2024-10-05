/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler_cgi.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:35:44 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/05 19:05:05 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"
#include "string.h"
#include <csignal>
#include <cstdlib>
#include <ctime>
#include <sched.h>
#include <sys/wait.h>
#include <unistd.h>
#include <unistd.h>
#include <vector>
#include <csignal>

static void setPipe(int *fd, int end) {
	if (dup2(fd[end], end) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1) {
		std::cerr << "Error: could not open pipe for cgi output\n";
		throw (1);
	}
}

// void	alarm_handler(int _sig) { (void)_sig; throw (1); }

// forks process - child execve's script
// return true if success -> false if failed
CgiResult EventHandler::startCGI(int clientFd, std::vector<std::string> arguments) {
	pid_t pid;
	int fd[2];

	if (pipe(fd) == -1) {
		std::cerr << "Error: could not establish pipe for CGI output\n";
		return ERROR;
	}
	pid = fork();
	if (pid == -1) {
		std::cerr << "Error: could not fork process in CGI output\n";
		close(fd[0]);
		close(fd[1]);
		return ERROR;
	}
	if (pid == 0) {
		setPipe(fd, STDOUT_FILENO);
		char *const args[] =
		{
			const_cast<char *>(arguments[0].c_str()),
			const_cast<char *>(arguments[1].c_str()),
			NULL
		};

		// signal(SIGALRM, alarm_handler);
		// alarm(TIMEOUT);
		execve(arguments.front().c_str(), args, environ);
		std::cerr << "Error: could not execute cgi script\n";
		throw (1);
	} else {
		close(fd[1]);
		time_t start_time = std::time(NULL);
		while (1)
		{
			int status;
			pid_t result = waitpid(pid, &status, WNOHANG);
			if (result < 0)
			{
				std::cerr << "WAITPID FAILED: its ok though" << std::endl;
				return NOTFOUND;
				//throw (1);
			}
			else if (result == 0)
			{
				if (std::time(NULL) - start_time >= TIMEOUT)
				{
					std::cout << "child timeout after: " << TIMEOUT << std::endl;
					kill(pid, SIGTERM);
					waitpid(pid, NULL, 0);
					return TIMEDOUT;
				}
			}
			else
			{
				bool code = result < 0 || !WIFEXITED(status) || WEXITSTATUS(status) != 0;

				std::cout << "CGI EXIT STATUS: " << code << std::endl;
				std::cout << "Response sent to client.\n";

				if (!code)
				{
					std::cerr << "HERHERHERHERHERHERE" << std::endl;
					addToEpoll(fd[0]);
					_cgiManager.addCgiProcess(clientFd, fd[0], pid);
					_openConns[fd[0]] = EP_CGI;
					return SUCCESS;
				}
				else
				{
					close(fd[0]);
					return ERROR;
				}
			}
		}

	}
	return ERROR;
}

