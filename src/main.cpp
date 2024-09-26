/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 15:11:44 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/26 17:55:06 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"

int main(void) {
	try {
		Server s;
		EventHandler e;
		if (!e.addSocketToEpoll(s.getSockFd()))
			return 1;
		e.epollLoop(s);
	} catch (std::exception &e) {
		std::cerr << e.what();
	}
	
	return 0;
}
