/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:20:55 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/24 13:18:57 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Epoll.hpp"

Epoll::Epoll(Socket& sock)
{
	_epollFd = epoll_create1(0);
	if (_epollFd == -1) {
		std::cerr << "Error could not create epoll instance.\n";
	}
	_event.events = EPOLLIN;
	_event.data.fd = sock.getSockFd();
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, sock.getSockFd(), &_event) == -1) {
		std::cerr << "Error could not add server socket to epoll set\n";
	} else {
		std::cout << "Successfully added socket to epoll set\n";
	}
}

Epoll::~Epoll()
{
	if (_epollFd != -1) {
		close(_epollFd);
	}
}

