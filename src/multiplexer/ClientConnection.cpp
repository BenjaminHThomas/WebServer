/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnection.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:58:12 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/05 16:04:36 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientConnection.hpp"

ClientConnection::ClientConnection(int fd, const Config &config) :
	_clientFd(fd),
	_bytesSent(0),
	_responseReady(false),
	_cgiFailed(false),
	_cgi(false),
	_reqType(NONCHUNKED),
	_config(config)
{
}

 ClientConnection::ClientConnection(const ClientConnection& other) :
		_clientFd(dup(other._clientFd)),
		_requestBuffer(other._requestBuffer),
		_responseBuffer(other._responseBuffer),
		_cgiBuffer(other._cgiBuffer),
		_bytesSent(other._bytesSent),
		_responseReady(other._responseReady),
		_cgiFailed(false),
		_cgi(false),
		_reqType(NONCHUNKED),
		_config(other._config)
{
}

ClientConnection& ClientConnection::operator=(const ClientConnection& other) {
	if (this != &other) {
		closeConnection();
		_clientFd = dup(other._clientFd);
		_requestBuffer = other._requestBuffer;
		_cgiBuffer = other._cgiBuffer;
		_responseBuffer = other._responseBuffer;
		_responseReady = false;
		_cgiFailed = false;
		_cgi = false;
		_reqType = other._reqType;
	}
	return *this;
}

void ClientConnection::resetData() {
	_requestBuffer.clear();
	_responseBuffer.clear();
	_cgiBuffer.clear();
	_bytesSent = 0;
	_responseReady = false;
	_cgiFailed = false;
	_cgi = false;
	_reqType = NONCHUNKED;
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