/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 13:01:07 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/02 14:16:52 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "config.hpp"
#ifndef SERVER_HPP
# define SERVER_HPP

#define MAX_EVENTS 1024
#define MAX_CLIENTS 500

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>

class Server
{
	private:
		Config &_config;

		int _sockFd;
		int _port;
		int _maxClients;
		int _maxEvents;

	public:
		int getSockFd() const;
		int getPort() const;
		int getMaxClients() const;
		int getMaxEvents() const;
		const Config& getConfig() const;

	public:
		class socketCreationFailure;
		class socketBindFailure;
		class socketListenFailure;

	public:
		Server (const Server &socket);
		Server (Config &socket);
		~Server ();
		Server & operator=(const Server &other);
};

#endif /* SERVER_HPP */
