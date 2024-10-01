/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:48:37 by okoca             #+#    #+#             */
/*   Updated: 2024/10/01 09:57:46 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "json.hpp"
#include <netdb.h>
#include <stdint.h>
#include <stdint.h>
#include <map>
#include <set>
#include <sys/types.h>
#include <vector>

class Config
{
public:
	class BadValue;
public:
	struct HTTP_METHODS
	{
		enum Value
		{
			GET,
			POST,
			DELETE
		};
	};

	struct ERROR_PAGES
	{
		enum Value
		{
			E400,
			E404,
			E405,
			E500,
		};
	};

	struct Routes
	{
		std::string	path;
		std::string	index;
		std::string	directory;
		std::string	upload;
		bool		dir_listing;
		bool		has_cgi;

		std::set<HTTP_METHODS::Value>		methods;
		std::map<std::string, std::string>	cgi;

		Routes();
	};
public:
	typedef std::vector<Config>			config_list;
private:
	std::string	_name;
	std::string	_host;
	uint64_t	_address;
	uint64_t	_max_body_size;
	int			_port;
	addrinfo	*_addr;

	std::map<ERROR_PAGES::Value, std::string>	_error_pages;

	std::vector<Routes>	routes;


public:
	Config();
	~Config();

	Config &operator=(const Config& data);
public:
	static config_list init(JsonValue json);

private:
	static ERROR_PAGES::Value match_error_page(const std::string &page);
};