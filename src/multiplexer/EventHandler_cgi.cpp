/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler_cgi.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:35:44 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/01 12:50:01 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"

static void setPipe(int *fd, int end) {
	if (dup2(fd[end], end) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1) {
		std::cerr << "Error: could not open pipe for cgi output\n";
		exit(1);
	}
}

// forks process - child execve's script
void EventHandler::startCGI(int clientFd, std::string fname) {
	pid_t pid;
	int fd[2];

	if (pipe(fd) == -1) {
		std::cerr << "Error: could not establish pipe for CGI output\n";
		return ;
	}
	pid = fork();
	if (pid == -1) {
		std::cerr << "Error: could not fork process in CGI output\n";
		close(fd[0]);
		close(fd[1]);
		return ;
	}
	if (pid == 0) {
		setPipe(fd, STDOUT_FILENO);
		execve(fname.c_str(), _av, _env);
		std::cerr << "Error: could not execute cgi script\n";
		exit(1);
	} else {
		close(fd[1]);
		addToEpoll(fd[0]);
		_cgiManager.addCgiProcess(clientFd, fd[0], pid);
	}
	waitpid(pid, NULL, 0);
	std::cout << "Response sent to client.\n";
}

