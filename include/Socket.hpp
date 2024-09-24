/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 13:01:07 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/24 13:10:26 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <iostream>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>

class Socket
{
	private:
		int _sockFd;
		const int _port;
		const int _maxClients;
		const int _maxEvents;
		struct sockaddr_in _addr;

	public:
		int getSockFd() const;
		int getPort() const;
		int getMaxClients() const;
		int getMaxEvents() const;

	public:
		Socket (int port = 8080, int maxClients = 10, int maxEvents = 10);
		Socket (const Socket &socket);
		~Socket ();
};

#endif /* SOCKET_HPP */
