/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 15:11:44 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/02 13:52:19 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"
#include "Request.hpp"

#include "cluster.hpp"
#include "config.hpp"
#include "json.hpp"
#include "parser.hpp"
#include <csignal>
#include <exception>
#include <temp.hpp>

void	handle_sigint(int _sig) { throw _sig; }

int main(int ac, char **av)
{
	if (ac != 2)
		return 1;

	std::ifstream input(av[1]);
	if (!input)
		return 1;
	try
	{
		JsonValue json  = JSONParser::parse(input);
		Cluster cluster(json);
		signal(SIGINT, handle_sigint);

		cluster.start();
	}
	catch (int e)
	{
		std::cerr << "\nexit successful..." << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr << "ERROR ERROR" << e.what();
	}

	return 0;
}
