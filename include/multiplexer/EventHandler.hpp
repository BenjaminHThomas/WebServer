/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:21:12 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/02 11:02:24 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
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

#define BUFFER_SIZE 30720

extern char **environ;

class ClientConnection;
class CGIManager;

class EventHandler
{
	private:
		int _epollFd;
		std::map<int, Server*> _servers;
		std::map<int, ClientConnection*> _clients;
		CGIManager _cgiManager;

	public:
		class epollInitFailure;
		void setNonBlock(int fd);
		bool addToEpoll(int fd);
		bool deleteFromEpoll(int fd);
		void addClient(int clientFd);
		void addServer(Server & s);
		void handleNewConnection(Server & s);
		void handleClientRequest(int clientFd);
		void handleResponse(int clientFd);
		void epollLoop(void);
		void changeToWrite(int clientFd);
		void changeToRead(int clientFd);
		bool isResponseComplete(int clientFd);
		void startCGI(int clientFd, std::string fname);
		void checkCompleteCGIProcesses(void);

	public:
		class epollWaitFailure;

	public:
		EventHandler ();
		~EventHandler ();
};

#endif /* EVENTHANDLER_HPP */
