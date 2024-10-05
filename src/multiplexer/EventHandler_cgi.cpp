/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler_cgi.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:35:44 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/05 22:10:30 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"
#include "Request.hpp"
#include "string.h"
#include <cctype>
#include <csignal>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <sched.h>
#include <sys/wait.h>
#include <unistd.h>
#include <unistd.h>
#include <vector>
#include <csignal>
#include <stdlib.h>

static void setPipe(int *fd, int end) {
	if (dup2(fd[end], end) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1) {
		std::cerr << "Error: could not open pipe for cgi output\n";
		throw (1);
	}
}

std::string process_header_field(const std::string &s)
{
	std::string processed(s);
	for (std::string::iterator it = processed.begin(); it < processed.end(); it++)
	{
		if (*it == '-')
		{
			*it = '_';
			continue;
		}
		*it = std::toupper(*it);
	}
	processed.insert(0, "HTTP_");
	return processed;
}

void EventHandler::handle_environment(int clientFd, const std::string &arg)
{
	Request req(_clients.at(clientFd)->_requestBuffer);
	environ = NULL;

	setenv("PATH_INFO", arg.c_str(), 1);
	setenv("REQUEST_METHOD", req.getMethod().c_str(), 1);
	if (!req.getQueryString().empty())
		setenv("QUERY_STRING", req.getQueryString().c_str(),1);
	try
	{
		for (std::map<std::string, std::string>::const_iterator it = req.getHeaders().begin(); it != req.getHeaders().end(); it++)
		{
			std::string s(process_header_field(it->first));
			std::cout << "zatiri zort zort: " << s <<": " << it->second << std::endl;
			setenv(s.c_str(), it->second.c_str(), 1);
		}
		if (req.getMethod() == "POST")
		{
			setenv("CONTENT_TYPE", req.getHeaderValue("Content-Type").c_str(), 1);
			setenv("CONTENT_LENGTH", req.getHeaderValue("Content-Length").c_str(), 1);
		}
		setenv("SERVER_NAME", req.getHeaderValue("Host").c_str(), 1);
	}
	catch (const std::exception &e)
	{
		std::cerr << "[log] setenv, can ignore" << std::endl;
	}
}

// void	alarm_handler(int _sig) { (void)_sig; throw (1); }

// forks process - child execve's script
// return true if success -> false if failed
bool EventHandler::startCGI(int clientFd, std::vector<std::string> arguments) {
	pid_t pid;
	int fd[2];

	if (pipe(fd) == -1) {
		std::cerr << "Error: could not establish pipe for CGI output\n";
		return false;
	}
	pid = fork();
	if (pid == -1) {
		std::cerr << "Error: could not fork process in CGI output\n";
		close(fd[0]);
		close(fd[1]);
		return false;
	}
	if (pid == 0) {
		setPipe(fd, STDOUT_FILENO);
		char *const args[] =
		{
			const_cast<char *>(arguments[0].c_str()),
			const_cast<char *>(arguments[1].c_str()),
			NULL
		};

		handle_environment(clientFd, arguments[0]);

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
				return false;
				//throw (1);
			}
			else if (result == 0)
			{
				if (std::time(NULL) - start_time >= TIMEOUT)
				{
					std::cout << "child timeout after: " << TIMEOUT << std::endl;
					kill(pid, SIGTERM);
					waitpid(pid, NULL, 0);
					return false;
				}
			}
			else
			{
				bool code = result < 0 || !WIFEXITED(status) || WEXITSTATUS(status) != 0;

				std::cout << "CGI EXIT STATUS: " << code << std::endl;
				std::cout << "Response sent to client.\n";

				if (!code)
				{
					addToEpoll(fd[0]);
					_cgiManager.addCgiProcess(clientFd, fd[0], pid);
					_openConns[fd[0]] = EP_CGI;
					return true;
				}
				else
				{
					close(fd[0]);
					return false;
				}
			}
		}

	}
	return false;
}

