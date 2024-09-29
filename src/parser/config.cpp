/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:55:36 by okoca             #+#    #+#             */
/*   Updated: 2024/09/29 16:55:11 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include "json.hpp"
#include <iostream>

bool Config::validator(JsonValue json)
{
	try
	{
		std::cout << json[1]["name"] << std::endl;
		std::cout << json[1]["host"] << std::endl;
		std::cout << json[1]["port"] << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "config error: " << e.what() << std::endl;
	}

	return true;
}

Config::~Config()
{}