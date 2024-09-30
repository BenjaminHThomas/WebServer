/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:55:36 by okoca             #+#    #+#             */
/*   Updated: 2024/09/30 10:02:59 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include "json.hpp"
#include <iostream>

std::vector<Config> Config::init(JsonValue json)
{
	Config::config_list cfg;
	try
	{
		for (JsonValue::const_iter_arr it = json.begin_arr(); it < json.end_arr(); it++)
		{
			std::cout << (*it)["name"] << "\n";
			std::cout << (*it)["host"] << "\n";
			std::cout << (*it)["port"] << "\n";
			std::cout << "--------" << std::endl;
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "config error: " << e.what() << std::endl;
	}

	return cfg;
}

Config::~Config()
{}