/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 13:01:07 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/01 15:55:50 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

#define MAX_EVENTS 1024
#define MAX_CLIENTS 500

#include <iostream>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>

class Server
{
	private:
		int _sockFd;
		int _port;
		int _maxClients;
		int _maxEvents;
		struct sockaddr_in _addr;

	public:
		int getSockFd() const;
		int getPort() const;
		int getMaxClients() const;
		int getMaxEvents() const;

	public:
		class socketCreationFailure;
		class socketBindFailure;
		class socketListenFailure;

	public:
		Server (int port = 8080);
		Server (const Server &socket);
		~Server ();
		Server & operator=(const Server &other);
};

#endif /* SERVER_HPP */
