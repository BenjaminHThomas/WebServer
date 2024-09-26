/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:20:55 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/26 16:32:58 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"

class EventHandler::epollInitFailure : public std::exception {
	public:
		const char* what() const throw() {
			return "Error: could not initialise epoll with server socket\n";
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
	ev.events = EPOLLIN | EPOLLET;
	if (epoll_ctl(_epollFd, EPOLL_CTL_MOD, clientFd, &ev) == -1) {
		std::cerr << "Error: could not mark fd " << clientFd << " as EPOLLIN\n";
	}
}

void EventHandler::changeToWrite(int clientFd) {
	struct epoll_event ev;
	ev.data.fd = clientFd;
	ev.events = EPOLLOUT | EPOLLET;
	if (epoll_ctl(_epollFd, EPOLL_CTL_MOD, clientFd, &ev) == -1) {
		std::cerr << "Error: could not mark fd " << clientFd << " as EPOLLOUT\n";
	}
}

void EventHandler::addSocketToEpoll(Server & s) {
	int sfd = s.getSockFd();
	setNonBlock(sfd);
	
	struct epoll_event ev;
	ev.data.fd = sfd;
	ev.events = EPOLLIN | EPOLLET;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, sfd, &ev) == -1) {
		std::cerr << "Error: could not mark fd " << sfd << " as EPOLLIN\n";
	}
}

void EventHandler::addSocketToEpoll(int fd) {
	setNonBlock(fd);
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN | EPOLLET;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		std::cerr << "Error: could not mark fd " << fd << " as EPOLLIN\n";
	}
}

void EventHandler::addClient(int clientFd) {
	ClientConnection conn(clientFd);
	_clients[clientFd] = conn;
}

void EventHandler::handleNewConnection(Server & s) {
	int serverFd = s.getSockFd();
	std::cout << "New connection from " << serverFd << "\n";

	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	//accept new connection
	int clientFd = accept(serverFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
	if (clientFd == -1) {
		std::cerr << "Error: could not accept new connection from fd " << serverFd << "\n";
		return ;
	}
	addSocketToEpoll(clientFd);
	addClient(clientFd);
}

bool EventHandler::isResponseComplete(int clientFd) {
	std::string buff = _clients[clientFd]._requestBuffer;
	size_t pos = buff.find("\r\n\r\n");
	if (pos != std::string::npos)
		return true;
	return false;
}

// Read all data from the client
void EventHandler::handleClientRequest(int clientFd) {
	char buffer[BUFFER_SIZE];
	int bytes_read;

	std::cout << "Handling client request from " << clientFd << "\n";

	while (1) {
		bytes_read = read(clientFd, buffer, sizeof(buffer) - 1);
		if (bytes_read < 0) {
			break ;
		}
		buffer[bytes_read] = 0;
		_clients[clientFd]._requestBuffer += buffer;
		if (isResponseComplete(clientFd)) {
			std::cout << "Recieved request:\n" << _clients[clientFd]._requestBuffer << "\n";
			changeToWrite(clientFd);
			break ;
		}
	}
}

void EventHandler::handleResponse(int clientFd) {
	// replace the below
	std::cout << "Sending response to client " << clientFd << "\n";
	_clients[clientFd].resetData();
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
		int numEvents = epoll_wait(_epollFd, eventQueue, MAX_EVENTS, -1);
		if (numEvents == -1) {
			std::cerr << "Failed to wait for events.\n";
			break ;
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