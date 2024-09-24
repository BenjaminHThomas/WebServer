/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:21:12 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/24 13:14:21 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef EPOLL_HPP
# define EPOLL_HPP

#include "Socket.hpp"

class Epoll
{
	private:
		int _epollFd;
		struct epoll_event _event;

	public:
		Epoll (Socket& sock);
		~Epoll ();
};

#endif /* EPOLL_HPP */
