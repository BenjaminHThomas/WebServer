/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnection.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:58:12 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/27 13:44:19 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientConnection.hpp"

ClientConnection::ClientConnection(int fd) : 
	_clientFd(fd),
	_bytesSent(0),
	_responseReady(false)
{
}

ClientConnection::ClientConnection(void) :
	_clientFd(-1),
	_bytesSent(0),
	_responseReady(false) 
{
}

 ClientConnection::ClientConnection(const ClientConnection& other) :
		_clientFd(other._clientFd),
		_requestBuffer(other._requestBuffer),
		_responseBuffer(other._responseBuffer),
		_bytesSent(other._bytesSent),
		_responseReady(other._responseReady)
{
}

ClientConnection& ClientConnection::operator=(const ClientConnection& other) {
	if (this != &other) {
		closeConnection();
		_clientFd = other._clientFd;
		_requestBuffer = other._requestBuffer;
		_responseBuffer = other._responseBuffer;
		_responseReady = false;
	}
	return *this;
}

void ClientConnection::resetData() {
	_requestBuffer.clear();
	_responseBuffer.clear();
	_bytesSent = 0;
	_responseReady = false;
}

void ClientConnection::closeConnection() {
	if (_clientFd != -1) {
		std::cout << "Closing connection.\n";
		close(_clientFd);
		_clientFd = -1;
	}
}

ClientConnection::~ClientConnection() {
	closeConnection();
}