/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:20:55 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/02 09:42:16 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"
#include "Request.hpp"
#include "Response.hpp"

void cgiOut(int clientFd, char **av, char **env);

class EventHandler::epollInitFailure : public std::exception {
	public:
		const char* what() const throw() {
			return "Error: could not initialise epoll\n";
		}
};

class EventHandler::epollWaitFailure : public std::exception {
	public:
		const char* what() const throw() {
			return "Error: epoll_wait failed\n";
		}
};

EventHandler::EventHandler(char **av, char **env)
{
	_epollFd = epoll_create1(0);
	if (_epollFd == -1) {
		throw epollInitFailure();
	}
	_av = av;
	_env = env;
}

EventHandler::~EventHandler()
{
	if (_epollFd != -1) {
		close(_epollFd);
	}
	std::map<int, ClientConnection*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it) {
		delete it->second;
	}
	_clients.clear();
}

void EventHandler::setNonBlock(int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		std::cerr << "Error: couldn't set fd to non-blocking\n";
		return ;
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		std::cerr << "Error: couldn't set fd to non-blocking\n"; 
	}
}

void EventHandler::changeToRead(int clientFd) {
	struct epoll_event ev;
	ev.data.fd = clientFd;
	ev.events = EPOLLIN;
	if (epoll_ctl(_epollFd, EPOLL_CTL_MOD, clientFd, &ev) == -1) {
		std::cerr << "Error: could not mark fd " << clientFd << " as EPOLLIN\n";
	}
}

void EventHandler::changeToWrite(int clientFd) {
	struct epoll_event ev;
	ev.data.fd = clientFd;
	ev.events = EPOLLOUT;
	if (epoll_ctl(_epollFd, EPOLL_CTL_MOD, clientFd, &ev) == -1) {
		std::cerr << "Error: could not mark fd " << clientFd << " as EPOLLOUT\n";
	}
}

bool EventHandler::addToEpoll(int fd) {
	setNonBlock(fd);
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		std::cerr << "Error: could not mark fd " << fd << " as EPOLLIN\n";
		return false;
	}
	return true;
}

bool EventHandler::deleteFromEpoll(int fd) {
	if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL) == -1) {
		std::cerr << "Error: could not delete fd " << fd << "\n";
		return false;
	}
	return true;
}

void EventHandler::addClient(int clientFd) {
	ClientConnection* conn = new ClientConnection(clientFd);
	_clients[clientFd] = conn;
}

void EventHandler::addServer(Server & s) {
	int serverFd = s.getSockFd();
	std::map<int, Server*>::iterator it = _servers.find(serverFd);
	if (it == _servers.end()) {
		if (!addToEpoll(serverFd)) {
			return ;
		}
		_servers[serverFd] = &s;
	}
}

void EventHandler::handleNewConnection(Server & s) {
	int serverFd = s.getSockFd();
	std::cout << "New connection from " << serverFd << "\n";

	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	int clientFd = accept(serverFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
	if (clientFd == -1) {
		std::cerr << "Error: could not accept new connection from fd " << serverFd << "\n";
		return ;
	}
	if (!addToEpoll(clientFd)) {
		close(clientFd);
		return ;
	}
	addClient(clientFd);
}

bool EventHandler::isResponseComplete(int clientFd) {
	std::string buff = _clients[clientFd]->_requestBuffer;
	size_t pos = buff.find("\r\n\r\n");
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

// Read all data from the client
void EventHandler::handleClientRequest(int clientFd) {
	char buffer[BUFFER_SIZE];
	int bytes_read;

	std::cout << "Handling client request from " << clientFd << "\n";
	bytes_read = read(clientFd, buffer, sizeof(buffer) - 1);
	if (bytes_read <= 0) {
		std::cerr << "Error: failed to read or client closed connection.\n";
		deleteFromEpoll(clientFd);
		delete _clients.at(clientFd);
		return ;
	}
	buffer[bytes_read] = 0;
	_clients.at(clientFd)->_requestBuffer.append(buffer);
	if (isResponseComplete(clientFd)) {
		std::cout << "Recieved request:\n" << _clients.at(clientFd)->_requestBuffer << "\n";
		changeToWrite(clientFd);
	}
}

// Write response to the client
void EventHandler::handleResponse(int clientFd) {
	std::cout << "Sending response to client " << clientFd << "\n";
	// 1. HTTP Parse the reqesut Buffer
	Request	rqs(_clients.at(clientFd)->_requestBuffer);
	// Request	rqs(_clients[clientFd]->_requestBuffer);
	
	// 2. Check if it is a cgi or not using Config and Path
	// 		- Maybe do this part in Response
		
	// 3. Generate Response based on Request object
	/* A Response object to be created and feed output */
	Response rsp(rqs);
	_clients.at(clientFd)->_responseBuffer.append(rsp.generateResponse());
	// _clients[clientFd]->_responseBuffer += rsp.generateResponse();
	
	// 4. Write to the clientFD with reponse string
	//		- Use string.at() instead of indexing.
	std::cout << _clients.at(clientFd)->_responseBuffer << std::endl;
	write(clientFd, _clients.at(clientFd)->_responseBuffer.c_str(), _clients.at(clientFd)->_responseBuffer.length());
	// 5. clear the buff in this clientFD
	_clients.at(clientFd)->resetData();
	// _clients[clientFd]->resetData();
	
	
	// cgiOut(clientFd, "cgi_bin/tester.cgi");
	changeToRead(clientFd);
}

void EventHandler::checkCompleteCGIProcesses(void) {
	std::map<int, CGIInfo*>::iterator it;
	for (it = _cgiManager._cgiProcesses.begin();
			it != _cgiManager._cgiProcesses.end();
			++it) {
		if (it->second->isFinished) {
			CGIInfo *info = it->second;
			int clientFd = info->clientFd;
			_clients[clientFd]->_responseBuffer = info->output;
			deleteFromEpoll(info->pipeFd);
			_cgiManager.deleteFromCGI(info->pipeFd);
			changeToWrite(clientFd);
		}
	}
}

void EventHandler::epollLoop(void) {
	struct epoll_event eventQueue[MAX_EVENTS];

	while (1) {
		int numEvents = epoll_wait(_epollFd, eventQueue, MAX_EVENTS, 0);
		if (numEvents == -1) {
			throw epollWaitFailure();
		}
		for (int i = 0; i < numEvents; ++i) {
			// New connection from server
			if (_servers.find(eventQueue[i].data.fd) != _servers.end()) {
				handleNewConnection(*_servers[eventQueue[i].data.fd]);
			// Output from CGI needs to be read in
			} else if (_cgiManager.isInManager(eventQueue[i].data.fd)) {
				_cgiManager.readCGIOutput(eventQueue[i].data.fd);
			// Parse client request
			} else if (eventQueue[i].events & EPOLLIN) {
				handleClientRequest(eventQueue[i].data.fd);
			// Send response to client
			} else if (eventQueue[i].events & EPOLLOUT) {
				handleResponse(eventQueue[i].data.fd);
			}
		}
		checkCompleteCGIProcesses();
	}
}
