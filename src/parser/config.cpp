/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:55:36 by okoca             #+#    #+#             */
/*   Updated: 2024/09/30 11:57:32 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include "json.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

std::vector<Config> Config::init(JsonValue json)
{
	Config::config_list cfg;
	try
	{
		for (JsonValue::const_iter_arr it = json.begin_arr(); it < json.end_arr(); it++)
		{
			Config e;
			const JsonValue &j = (*it);
			e._name = j["name"].as_string();
			e._host = j["host"].as_string();
			e._port = j["port"].as_number();
			e._max_body_size = j["max_body"].as_number();

			struct addrinfo hints, *result, *rp;
			int status;

			memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;

			status = getaddrinfo(e._host.c_str(), j["port"].as_string().c_str(), &hints, &result);
			if (status != 0) {
					std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
					throw std::runtime_error("invalid host in config");
			}

			for (rp = result; rp != NULL; rp = rp->ai_next)
			{
				std::cout << "Address family: " << rp->ai_family << std::endl;

				struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;

				char buf[100];
				inet_ntop(AF_INET, &(ipv4->sin_addr), buf, sizeof(buf));
				std::cout << "buf: " << buf << std::endl;
			}

			freeaddrinfo(result);

			// if (getaddrinfo() != 0)
			// 	throw std::runtime_error("invalid host in config");

			std::cout << (*it)["name"] << "\n";
			std::cout << (*it)["host"] << "\n";
			std::cout << (*it)["port"] << "\n";
			// std::cout << (*it)["routes"][0]["index"] << "\n";
			std::cout << "--------" << std::endl;
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "config error: " << e.what() << std::endl;
	}

	return cfg;
}

Config::Config() : _address(0), _max_body_size(0), _port(0)
{}

Config &Config::operator=(const Config& data)
{
	if (this != &data)
	{
		_address = data._address;
		_error_pages = data._error_pages;
		_host = data._host;
		_max_body_size = data._max_body_size;
		_name = data._name;
		_port = data._port;
	}
	return *this;
}

Config::~Config()
{}