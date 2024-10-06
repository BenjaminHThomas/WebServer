/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 15:11:44 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/05 09:39:05 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cluster.hpp"
#include "json.hpp"
#include "parser.hpp"
#include "utils.hpp"
#include <csignal>
#include <exception>
#include <iostream>
#include <string>

void	handle_sigint(int _sig) { throw (int)_sig; }
int main(int ac, char **av)
{
	if (ac != 2)
		return 1;

	if (end_with_json(av[1]) == false || is_directory(av[1]))
	{
		std::cerr << "error: the config has to be a json file" << std::endl;
		return 1;
	}
	std::ifstream input(av[1]);
	if (!input)
		return 1;
	try
	{
		signal(SIGINT, handle_sigint);
		JsonValue json  = JSONParser::parse(input);
		Cluster cluster(json);

		cluster.start();
	}
	catch (int v)
	{
		if (v == 1)
			return v;
		std::cerr << "\n("<< v << ") exit successful..." << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr << "error: " << e.what();
	}

	return 0;
}
