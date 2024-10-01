/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:55:36 by okoca             #+#    #+#             */
/*   Updated: 2024/10/01 10:16:14 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include "json.hpp"
#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <utility>

class Config::BadValue : public std::exception
{
private:
	std::string _message;
public:
	BadValue() : _message("bad value in config") {}
	BadValue(const std::string &s) : _message(s) {}

	const char * what() const throw()
	{
		return _message.c_str();
	}
	virtual ~BadValue() throw() {}
};

Config::ERROR_PAGES::Value Config::match_error_page(const std::string &page)
{
	if (page == "400")
		return Config::ERROR_PAGES::E400;
	else if (page == "404")
		return Config::ERROR_PAGES::E404;
	else if (page == "405")
		return Config::ERROR_PAGES::E405;
	else if (page == "500")
		return Config::ERROR_PAGES::E500;
	throw Config::BadValue();
}

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

			try
			{
				for (JsonValue::const_iter_obj err = j["error"].begin_obj(); err != j["error"].end_obj(); err++)
				{
					std::pair<ERROR_PAGES::Value, std::string> pair(match_error_page(err->first), err->second.as_string());
					e._error_pages.insert(pair);
					std::cout << match_error_page(err->first) << ", second" << err->second.as_string() << std::endl;
				}
			}
			catch (const Config::BadValue &e)
			{
				throw Config::BadValue();
			}
			catch (const std::exception &e)
			{
				std::cerr << "IN HERE, IGNORE, ITS GOOD THAT IT THROWS: " << e.what() << std::endl;
			}


			struct addrinfo hints, *r;
			std::memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;

			if (getaddrinfo(e._host.c_str(), j["port"].as_string().c_str(), &hints, &r) != 0)
					throw std::runtime_error("invalid host in config");
			e._addr = r;
			// socket(res->ai_family, res->ai_socktype, res->ai_protocol);
			// bind(sockfd, res->ai_addr, res->ai_addrlen);

			std::cout << (*it)["name"] << "\n";
			std::cout << (*it)["host"] << "\n";
			std::cout << (*it)["port"] << "\n";
			std::cout << (*it)["routes"][0]["index"] << "\n";
			std::cout << "--------" << std::endl;
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "config error: " << e.what() << std::endl;
	}

	return cfg;
}

Config::Config() : _address(0), _max_body_size(0), _port(0), _addr(NULL)
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
{
	if (_addr != NULL)
		freeaddrinfo(_addr);
}

Config::Routes::Routes() : dir_listing(true), has_cgi(false)
{
}