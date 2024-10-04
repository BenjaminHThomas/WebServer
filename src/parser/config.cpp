/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:55:36 by okoca             #+#    #+#             */
/*   Updated: 2024/10/04 20:28:47 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include "json.hpp"
#include <cstdlib>
#include <stdint.h>
#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <utility>

void Config::check_error_page(int page)
{
	if (page != 400 && page != 404 && page != 405 && page != 500)
		throw Config::BadValue();
}

addrinfo *Config::init_addrinfo(const std::string &host, const std::string &port)
{
	struct addrinfo hints, *r;
	std::memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(host.c_str(), port.c_str(), &hints, &r) != 0)
			throw Config::BadValue("invalid host in config");
	return r;
}

Config::Config(const JsonValue &j) : _addr(NULL)
{
	_name = j["name"].as_string();
	_host = j["host"].as_string();
	if (j["port"].as_number() < 0 || j["port"].as_number() > std::numeric_limits<uint16_t>::max())
		throw Config::BadValue();
	_port = j["port"].as_number();
	if (j["max_body"].as_number() < 0)
		throw Config::BadValue();
	_max_body_size = j["max_body"].as_number();

	try
	{
		for (JsonValue::const_iter_obj err = j["error"].begin_obj(); err != j["error"].end_obj(); err++)
		{
			check_error_page(std::atoi(err->first.c_str()));
			std::pair<int, std::string> pair(std::atoi(err->first.c_str()), err->second.as_string());
			_error_pages.insert(pair);
			std::cout << err->first << ", second" << err->second.as_string() << std::endl;
		}
	}
	catch (const Config::BadValue &e)
	{
		throw Config::BadValue();
	}
	catch (const std::exception &e)
	{
	}

	for (JsonValue::const_iter_arr it_route = j["routes"].begin_arr(); it_route < j["routes"].end_arr(); it_route++)
	{
		_routes.push_back(Routes(*it_route));
	}

	_addr = init_addrinfo(_host, j["port"].as_string());

	std::cout << j["name"] << "\n";
	std::cout << j["host"] << "\n";
	std::cout << j["port"] << "\n";
	std::cout << j["routes"][0]["index"] << "\n";
	std::cout << "--------" << std::endl;
}

Config::~Config()
{
	if (_addr != NULL)
		freeaddrinfo(_addr);
}

Config::Routes::Routes() : dir_listing(true)
{}

std::string Config::handle_directory(const std::string &s)
{
	std::string current = s;
	if (*current.rbegin() != '/')
		current += '/';
	return current;
}

Config::Routes::Routes(const JsonValue &j) : dir_listing(true)
{
	path = j["route"].as_string();
	index = j["index"].as_string();
	dir_listing = j["dir_listing"].as_bool();
	directory = handle_directory(j["directory"].as_string());
	upload = j["upload"].as_string();

	for (JsonValue::const_iter_arr it_method = j["methods"].begin_arr(); it_method < j["methods"].end_arr(); it_method++)
		methods.insert(it_method->as_string());

	try
	{
		const JsonValue &rt = j["cgi"];

		for (JsonValue::const_iter_arr it_cgi = rt.begin_arr(); it_cgi != rt.end_arr(); it_cgi++)
		{
			const JsonValue &c = (*it_cgi);
			if (std::distance(c.begin_obj(), c.end_obj()) != 2)
				throw Config::BadValue("too many cgi arguments");
			std::pair<std::string, std::string> el(c["extension"].as_string(), c["exec"].as_string());
			std::cout << "CGI: " << el.first << ", exec: " << el.second << std::endl;
			cgi.insert(el);
		}
	}
	catch (const std::out_of_range &e)
	{
		std::cerr << "IGNORE -> route doesnt contain any CGI: " << e.what() << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "CGI: " << e.what() << std::endl;
		throw e;
	}
}

const std::string& Config::get_name() const
{return _name; }

const std::string& Config::get_host() const
{return _host; }

uint64_t Config::get_address() const
{return _address; }

uint64_t Config::get_max_body_size() const
{return _max_body_size; }

int Config::get_port() const
{return _port; }

const addrinfo* Config::get_addr() const
{return _addr; }

const std::map<int, std::string>& Config::get_error_pages() const
{return _error_pages; }

const std::vector<Config::Routes>& Config::get_routes() const
{return _routes; }