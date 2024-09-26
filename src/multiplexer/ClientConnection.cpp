/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnection.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:58:12 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/26 12:03:51 by bthomas          ###   ########.fr       */
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

ClientConnection& ClientConnection::operator=(const ClientConnection& other) {
	if (this != &other) {
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
	close(_clientFd);
}

ClientConnection::~ClientConnection() {
	closeConnection();
}