/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:20:55 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/27 16:29:25 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"

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

EventHandler::EventHandler(void)
{
	_epollFd = epoll_create1(0);
	if (_epollFd == -1) {
		throw epollInitFailure();
	}
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
		delete _clients[clientFd];
		return ;
	}
	buffer[bytes_read] = 0;
	_clients[clientFd]->_requestBuffer += buffer;
	if (isResponseComplete(clientFd)) {
		std::cout << "Recieved request:\n" << _clients[clientFd]->_requestBuffer << "\n";
		changeToWrite(clientFd);
	}
}

void EventHandler::handleResponse(int clientFd) {
	// replace the below
	std::cout << "Sending response to client " << clientFd << "\n";
	_clients[clientFd]->resetData();
	const char* response =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: 13\r\n"
		"\r\n"
		"Hello, World!";
	write(clientFd, response, strlen(response));
	changeToRead(clientFd);
}

void EventHandler::epollLoop(Server & s) {
	struct epoll_event eventQueue[MAX_EVENTS];
	int serverFd = s.getSockFd();

	while (1) {
		// change from -1 after bug is fixed
		int numEvents = epoll_wait(_epollFd, eventQueue, MAX_EVENTS, -1);
		if (numEvents == -1) {
			throw epollWaitFailure();
		}
		if (numEvents != 0)
			std::cout << "Num events: " << numEvents << "\n";
		for (int i = 0; i < numEvents; ++i) {
			if (eventQueue[i].data.fd == serverFd) {
				handleNewConnection(s);
			} else if (eventQueue[i].events & EPOLLIN) {
				handleClientRequest(eventQueue[i].data.fd);
			} else if (eventQueue[i].events & EPOLLOUT) {
				handleResponse(eventQueue[i].data.fd);
			}
		}
	}
}