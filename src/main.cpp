/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 15:11:44 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/01 16:51:40 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"

#include "json.hpp"
#include "parser.hpp"
#include <temp.hpp>


int main(int ac, char **av, char **env)
{
	if (ac != 2)
		return 1;

	std::ifstream input(av[1]);
	if (!input)
		return 1;
	JsonValue json  = JSONParser::parse(input);

	std::cout << json << std::endl;

	try {
		Server s;
		EventHandler e(av, env);
		e.addServer(s);
		e.epollLoop();
	} catch (std::exception &e) {
		std::cerr << e.what();
	}

	return 0;
}
