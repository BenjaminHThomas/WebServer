/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 15:11:44 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/27 14:06:20 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"

int main(void) {
	try {
		Server s;
		EventHandler e;
		if (!e.addToEpoll(s.getSockFd()))
			return 1;
		e.epollLoop(s);
	} catch (std::exception &e) {
		std::cerr << e.what();
	}
	
	return 0;
}
