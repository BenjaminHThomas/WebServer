/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:48:37 by okoca             #+#    #+#             */
/*   Updated: 2024/10/02 13:19:54 by tsuchen          ###   ########.fr       */
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
	struct Routes
	{
		std::string	path;			// correspond to "route" in the config "routes". Use this to find the most matched routes to URL
		std::string	index;			// default file to open if there is file path is empty
		std::string	directory;		// default redirected directory Methods except for POST
		std::string	upload;			// default redirected directory for POST Method
		bool		dir_listing;	// flag to see if the output is a listing of directories
		bool		has_cgi;		// could use cgi.size() instead to check if there is any cgi

		std::set<std::string>		methods;
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

	std::map<std::string, std::string>	_error_pages;
	std::vector<Routes>					_routes;
public:
	Config(const JsonValue &j);
	~Config();

public:
	static void			check_error_page(const std::string &page);
	static addrinfo*	init_addrinfo(const std::string &host, const std::string &port);

// GETTERS
public:
	const std::string&	get_name() const;
	const std::string&	get_host() const;
	uint64_t			get_address() const;
	uint64_t			get_max_body_size() const;
	int					get_port() const;
	const addrinfo*		get_addr() const;

	const std::map<std::string, std::string>&	get_error_pages() const;
	const std::vector<Routes>&					get_routes() const;
};