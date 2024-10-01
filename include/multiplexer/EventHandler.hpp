/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:21:12 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/01 13:04:28 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef EVENTHANDLER_HPP 
# define EVENTHANDLER_HPP 

#include "Server.hpp"
#include "ClientConnection.hpp"
#include "CGIManager.hpp"
#include <fcntl.h>
#include <map>
#include <utility>
#include <cerrno> //temp, just for testing
#include <cstdlib>
#include <sys/wait.h>

#define BUFFER_SIZE 30720

class ClientConnection;

class EventHandler
{
	private:
		int _epollFd;
		std::map<int, ClientConnection*> _clients;
		CGIManager _cgiManager;
		char** _av;
		char** _env;

	public:
		class epollInitFailure;
		void setNonBlock(int fd);
		bool addToEpoll(int fd);
		bool deleteFromEpoll(int fd);
		void addClient(int clientFd);
		void handleNewConnection(Server & s);
		void handleClientRequest(int clientFd);
		void handleResponse(int clientFd);
		void epollLoop(void);
		void changeToWrite(int clientFd);
		void changeToRead(int clientFd);
		bool isResponseComplete(int clientFd);
		void startCGI(int clientFd, std::string fname);
		void sendCGIOutput(int fd);
		void checkCompleteCGIProcesses(void);

	public:
		class epollWaitFailure;

	public:
		EventHandler (char **av, char **env);
		~EventHandler ();
};

#endif /* EVENTHANDLER_HPP */
