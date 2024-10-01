/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:55:36 by okoca             #+#    #+#             */
/*   Updated: 2024/10/01 16:39:52 by tsuchen          ###   ########.fr       */
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

void Config::check_error_page(const std::string &page)
{
	if (page != "400" && page != "404" && page != "405" && page != "500")
		throw Config::BadValue();
}

addrinfo *Config::init_addrinfo(const std::string &host, const std::string &port)
{
	struct addrinfo hints, *r;
	std::memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(host.c_str(), port.c_str(), &hints, &r) != 0)
			throw std::runtime_error("invalid host in config");
	// to init socket and bind it:

	// socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	// bind(sockfd, res->ai_addr, res->ai_addrlen);
	return r;
}

std::vector<Config> Config::init(JsonValue json)
{
	std::vector<Config> cfg;
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
					check_error_page(err->first);
					std::pair<std::string, std::string> pair(err->first, err->second.as_string());
					e._error_pages.insert(pair);
					std::cout << err->first << ", second" << err->second.as_string() << std::endl;
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

			for (JsonValue::const_iter_arr it_route = j["routes"].begin_arr(); it_route < j["routes"].end_arr(); it_route++)
			{
				Routes route;

				const JsonValue &routes = *it_route;
				route.path = routes["route"].as_string();
				route.index = routes["index"].as_string();
				route.dir_listing = routes["dir_listing"].as_bool();
				route.directory = routes["directory"].as_string();
				route.upload = routes["upload"].as_string();

				for (JsonValue::const_iter_arr it_method = routes["methods"].begin_arr(); it_method < routes["methods"].end_arr(); it_method++)
					route.methods.insert(it_method->as_string());

				try
				{
					for (JsonValue::const_iter_obj it_cgi = routes["cgi"].begin_obj(); it_cgi != routes["cgi"].end_obj(); it_cgi++)
					{
						std::pair<std::string, std::string> el(it_cgi->first, it_cgi->second.as_string());
						route.cgi.insert(el);
					}
				}
				catch (const std::exception &e)
				{
					std::cerr << "error: " << e.what() << std::endl;
				}
			}

			e._addr = init_addrinfo(e._host, j["port"].as_string());

			std::cout << j["name"] << "\n";
			std::cout << j["host"] << "\n";
			std::cout << j["port"] << "\n";
			std::cout << j["routes"][0]["index"] << "\n";
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
{}