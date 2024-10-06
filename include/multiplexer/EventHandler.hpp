/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:21:12 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/06 11:12:26 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Request.hpp"
#include "cluster.hpp"
#include "config.hpp"
#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

#include "CGIManager.hpp"
#include "Server.hpp"
#include "ClientConnection.hpp"
#include <fcntl.h>
#include <map>
#include <cerrno> //temp, just for testing
#include <cstdlib>
#include <sys/wait.h>
#include <sstream>
#include <utils.hpp>

#define BUFFER_SIZE 30720
#define TIMEOUT 3

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

		const Cluster &_cluster;

	public:
		void setNonBlock(int fd);
		bool addToEpoll(int fd);
		bool deleteFromEpoll(int fd);
		void addClient(int clientFd, const Config &config);
		void addServer(Server & s);
		void handleNewConnection(Server & s);
		void handleClientRequest(int clientFd);
		void generateResponse(int clientFd);
		void handleResponse(int clientFd);
		void epollLoop(void);
		void changeToWrite(int clientFd);
		void changeToRead(int clientFd);
		bool isBodyTooBig(int clientFd);
		bool isResponseComplete(int clientFd);
		void sendInvalidResponse(int clientFd);
		// void startCGI(int clientFd, std::string fname);
		CgiResult startCGI(int clientFd, std::vector<std::string> fname);
		void checkCompleteCGIProcesses(void);

		const Config &get_config(const std::string &host, int clientFd) const;
		void handle_environment(const Request &req, const std::string &arg);

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
		EventHandler (const Cluster &cluster);
		~EventHandler ();
};

#endif /* EVENTHANDLER_HPP */
