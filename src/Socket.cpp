/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 13:01:27 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/24 13:21:04 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(int port, int maxClients, int maxEvents) :
	_port(port),
	_maxClients(maxClients),
	_maxEvents(maxEvents)
{
	_sockFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (_sockFd == -1) {
		std::cerr << "Error could not create socket.\n";
	}
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(_port);
	_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(_sockFd, (struct sockaddr*)&_addr, sizeof(_addr)) < 0) {
		std::cerr << "Error could not bind socket to port " << _port << "\n";
	}

	if (listen(_sockFd, 5) < 0) {
		std::cerr << "Error could not listen on port " << _port << "\n";
	} else {
		std::cout << "Server is listening on port " << _port << "...\n";
	}
}

Socket::Socket(const Socket &socket) :
	_port(socket._port),
	_maxClients(socket._maxClients),
	_maxEvents(socket._maxEvents)
{
	
}

Socket::~Socket()
{
	if (_sockFd != -1)
		close(_sockFd);
}

int Socket::getSockFd() const {
	return _sockFd;
}

int Socket::getPort() const {
	return _port;
}

int Socket::getMaxClients() const {
	return _maxClients;
}

int Socket::getMaxEvents() const {
	return _maxEvents;
}