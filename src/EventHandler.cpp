/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:20:55 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/24 13:18:57 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"

EventHandler::EventHandler(Server& s)
{
	_epollFd = epoll_create1(0);
	if (_epollFd == -1) {
		std::cerr << "Error could not create EventHandler instance.\n";
	}
	_event.events = EPOLLIN;
	_event.data.fd = s.getSockFd();
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, s.getSockFd(), &_event) == -1) {
		std::cerr << "Error could not add server socket to EventHandler set\n";
	} else {
		std::cout << "Successfully added socket to EventHandler set\n";
	}
}

EventHandler::~EventHandler()
{
	if (_epollFd != -1) {
		close(_epollFd);
	}
}

