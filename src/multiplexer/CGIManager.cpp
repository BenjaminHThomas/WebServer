/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIManager.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 12:10:49 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/07 11:23:52 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIManager.hpp"

CGIManager::CGIManager()
{
}

CGIManager::~CGIManager()
{
	// delete map
	std::map<int, CGIInfo*>::iterator it;
	for (it = _cgiProcesses.begin(); it != _cgiProcesses.end(); ++it) {
		close(it->first);
		delete it->second;
	}
}

void CGIManager::addCgiProcess(int clientFd, int pipeFd, pid_t pid) {
	CGIInfo *process = new CGIInfo;
	process->clientFd = clientFd;
	process->pipeFd = pipeFd;
	process->pid = pid;
	_cgiProcesses[pipeFd] = process;
	process->isFinished = false;
}

bool CGIManager::isInManager(int pipeFd) {
	std::map<int, CGIInfo*>::iterator res = _cgiProcesses.find(pipeFd);
	return res != _cgiProcesses.end();
}

void CGIManager::deleteFromCGI(int pipefd) {
	std::map<int, CGIInfo*>::iterator it = _cgiProcesses.find(pipefd);
	if (it != _cgiProcesses.end()) {
		close(pipefd);
		delete it->second;
		_cgiProcesses.erase(it);
	}
}

bool CGIManager::isResponseComplete(int pipefd) {
	if (!isInManager(pipefd))
		return true;
	std::string & buff = _cgiProcesses.at(pipefd)->output;
	std::string::size_type pos = buff.find("\r\n\r\n");
	if (pos == std::string::npos)
		return false;
	
	//Check if it's a POST request with a body
	size_t content_len_pos = buff.find("Content-Length: ");
	if (content_len_pos != std::string::npos) {
		size_t content_len_end = buff.find("\r\n", content_len_pos);
		std::string content_len_str = buff.substr(content_len_pos + 16, content_len_end - (content_len_pos + 16));
		int content_length = std::atoi(content_len_str.c_str());
		return buff.length() >= (pos + 4 + content_length);
	}
	return true;
}

void CGIManager::readCGIOutput(int pipefd) {
	char buffer[BUFFER_SIZE];
	int bytesRead;

	bytesRead = read(pipefd, buffer, sizeof(buffer) - 1);
	if (bytesRead <= 0) {
		_cgiProcesses[pipefd]->isFinished = true;
		return ;
	}
	buffer[bytesRead] = 0;
	_cgiProcesses[pipefd]->output += buffer;
	if (isResponseComplete(pipefd)) {
		_cgiProcesses[pipefd]->isFinished = true;
	}
}
