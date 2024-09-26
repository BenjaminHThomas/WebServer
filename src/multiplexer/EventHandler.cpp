/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:20:55 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/26 14:16:33 by bthomas          ###   ########.fr       */
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
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void EventHandler::addSocketToEpoll(Server & s) {
	int sfd = s.getSockFd();
	setNonBlock(sfd);
	struct epoll_event ev;
	ev.data.fd = sfd;
	ev.events = EPOLLIN | EPOLLET;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, sfd, &ev) == -1) {
		std::cerr << "Error: could not add fd " << sfd << " to epoll instance\n";
	}
}

void EventHandler::addSocketToEpoll(int fd) {
	setNonBlock(fd);
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN | EPOLLET;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		std::cerr << "Error: could not add fd " << fd << " to epoll instance\n";
	}
}

void EventHandler::addClient(int clientFd) {
	ClientConnection conn(clientFd);

	_clients[clientFd] = conn;
}

void EventHandler::handleNewConnection(Server & s) {
	int serverFd = s.getSockFd();
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

// Read all data from the client and process HTTP requests in Edge-Triggered mode
void EventHandler::handleClientRequest(int clientFd) {
	char buffer[BUFFER_SIZE];
	int bytes_read;

	while (1) {
		bytes_read = read(clientFd, buffer, sizeof(buffer) - 1);
		if (bytes_read <= 0) {
			if (bytes_read == 0) // this is wrong, need to check for "connection: close" header instead
				close(clientFd);
			break ;
		}
		buffer[bytes_read] = 0;
		std::cout << "Recieved request:\n" << buffer << "\n";

		// replace the below
		const char* response =
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/plain\r\n"
			"Content-Length: 13\r\n"
			"\r\n"
			"Hello, World!";
		write(clientFd, response, strlen(response));
	}
}

void EventHandler::epollLoop(Server & s) {
	struct epoll_event eventQueue[MAX_EVENTS];
	int serverFd = s.getSockFd();

	while (1) {
		int numEvents = epoll_wait(_epollFd, eventQueue, MAX_EVENTS, 0);
		if (numEvents == -1) {
			std::cerr << "Failed to wait for events.\n";
			break ;
		}
		for (int i = 0; i < numEvents; ++i) {
			if (eventQueue[i].data.fd == serverFd) {
				handleNewConnection(s);
			} else {
				//if (eventQueue[i].events & EPOLLIN)
					handleClientRequest(eventQueue[i].data.fd);
				//else if (eventQueue[i].events & EPOLLOUT)
					//handleResponse(eventQueue[i].data.fd);
			}
		}
	}
}