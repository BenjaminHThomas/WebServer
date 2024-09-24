/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 13:01:27 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/24 17:47:53 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

class Server::socketCreationFailure : public std::exception {
	public:
		const char* what() const throw() {
			return "Error: could not create socket.\n";
		}
};

class Server::socketBindFailure : public std::exception {
	public:
		const char* what() const throw() {
			return "Error: could not bind socket.\n";
		}
};

class Server::socketListenFailure : public std::exception {
	public:
		const char* what() const throw() {
			return "Error: could not listen on port.\n";
		}
};

Server::Server(int port, int maxClients, int maxEvents) :
	_port(port),
	_maxClients(maxClients),
	_maxEvents(maxEvents)
{
	// non-blocking TCP socket
	_sockFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (_sockFd == -1) {
		throw socketCreationFailure();
	}
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET; // IPv4
	_addr.sin_port = htons(_port); // Converts port number to network byte order
	_addr.sin_addr.s_addr = INADDR_ANY; // allows connection from any IP

	// attaches socket to port
	if (bind(_sockFd, (struct sockaddr*)&_addr, sizeof(_addr)) < 0) {
		throw socketBindFailure();
	}
	// wait for incoming connections
	if (listen(_sockFd, 5) < 0) {
		throw socketListenFailure();
	}
	std::cout << "Server is listening on port " << _port << "...\n";
}

Server::Server(const Server &s) : Server(s._port, s._maxClients, s._maxEvents)
{
}

Server::~Server()
{
	if (_sockFd != -1)
		close(_sockFd);
}

int Server::getSockFd() const {
	return _sockFd;
}

int Server::getPort() const {
	return _port;
}

int Server::getMaxClients() const {
	return _maxClients;
}

int Server::getMaxEvents() const {
	return _maxEvents;
}