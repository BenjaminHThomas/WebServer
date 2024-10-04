/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:21:12 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/04 15:37:17 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "config.hpp"
#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# define TIMEOUT 5

#include "CGIManager.hpp"
#include "Server.hpp"
#include "ClientConnection.hpp"
#include <fcntl.h>
#include <map>
#include <cerrno> //temp, just for testing
#include <cstdlib>
#include <sys/wait.h>
#include <sstream>

#define BUFFER_SIZE 30720
#define TIMEOUT 5

extern char **environ;

class ClientConnection;
class CGIManager;

enum reqType {
			CHUNKED,
			NONCHUNKED
		};
class EventHandler
{
	private:
		enum CONNTYPE {
			EP_SERVER,
			EP_CLIENT,
			EP_CGI
		};


	private:
		int _epollFd;
		std::map<int, Server*> _servers;
		std::map<int, ClientConnection*> _clients;
		CGIManager _cgiManager;
		std::map<int, CONNTYPE> _openConns;

	public:
		void setNonBlock(int fd);
		bool addToEpoll(int fd);
		bool deleteFromEpoll(int fd);
		void addClient(int clientFd, const Config &config);
		void addServer(Server & s);
		void handleNewConnection(Server & s);
		void handleClientRequest(int clientFd);
		void handleResponse(int clientFd);
		void epollLoop(void);
		void changeToWrite(int clientFd);
		void changeToRead(int clientFd);
		bool isBodyTooBig(int clientFd, int bytes_read);
		bool isResponseComplete(int clientFd);
		void sendInvalidResponse(int clientFd);
		// void startCGI(int clientFd, std::string fname);
		bool startCGI(int clientFd, std::vector<std::string> fname);
		void checkCompleteCGIProcesses(void);

	public:
		std::string::size_type getHeaderEndPos(int clientFd);
		bool isHeaderChunked(int clientFd);
		bool isChunkReqFinished(int clientFd);
		void cleanChunkedReq(int clientFd);
		std::string extractHeader(std::string & reqBuffer, std::string::size_type & headerEnd, int clientFd);
		std::string::size_type getChunkSize(std::string::size_type & chunkStart,
									std::string::size_type & chunkEnd,
									std::string & reqBuffer);

	public:
		class epollInitFailure;
		class epollWaitFailure;

	public:
		EventHandler ();
		~EventHandler ();
};

#endif /* EVENTHANDLER_HPP */
