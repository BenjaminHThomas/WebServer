/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:21:12 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/25 16:05:26 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef EVENTHANDLER_HPP 
# define EVENTHANDLER_HPP 

#include "Server.hpp"
#include <fcntl.h>

class EventHandler
{
	private:
		int _epollFd;

	public:
		class epollInitFailure;
		void setNonBlock(int fd);
		void addSocketToEpoll(Server & s);
		void addSocketToEpoll(int fd);
		void handleNewConnection(Server & s);
		void handleClientRequest(int clientFd);
		void handleResponse(int clientFd);
		void epollLoop(Server & s);

	public:
		EventHandler (void);
		~EventHandler ();
};

#endif /* EVENTHANDLER_HPP */
