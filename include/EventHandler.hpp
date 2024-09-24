/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:21:12 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/24 18:40:07 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef EVENTHANDLER_HPP 
# define EVENTHANDLER_HPP 

#include "Server.hpp"
#include <vector>

class EventHandler
{
	private:
		int _epollFd;
		std::vector<struct epoll_event> _events;

	public:
		EventHandler (Server& s);
		~EventHandler ();
};

#endif /* EVENTHANDLER_HPP */
