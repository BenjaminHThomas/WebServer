/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:48:37 by okoca             #+#    #+#             */
/*   Updated: 2024/10/06 17:09:51 by okoca            ###   ########.fr       */
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
	class BadValue: public std::exception
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
public:
	struct Routes
	{
		std::string	path;			// correspond to "route" in the config "routes". Use this to find the most matched routes to URL
		std::string	index;			// default file to open if there is file path is empty
		std::string	directory;		// default redirected directory Methods except for POST
		std::string	upload;			// default redirected directory for POST Method
		bool		dir_listing;	// flag to see if the output is a listing of directories
		bool		is_redirection;
		std::string	redirection;

		std::set<std::string>		methods;
		std::map<std::string, std::string>	cgi;

		Routes();
		Routes(const JsonValue &j);
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

	std::map<int, std::string>	_error_pages;
	std::vector<Routes>			_routes;
	const JsonValue				&_json;


	const std::string &set(const std::string &value_, const std::string &__default);
	static const std::string &set(const std::string &value, const std::string &__default, const JsonValue &j);
public:
	Config(const JsonValue &j);
	~Config();

private:
	static std::string handle_directory(const std::string &s);

public:
	static void			check_error_page(int page);
	static addrinfo*	init_addrinfo(const std::string &host, const std::string &port);

// GETTERS
public:
	const std::string&	get_name() const;
	const std::string&	get_host() const;
	uint64_t			get_address() const;
	uint64_t			get_max_body_size() const;
	int					get_port() const;
	const addrinfo*		get_addr() const;

	const std::map<int, std::string>&	get_error_pages() const;
	const std::vector<Routes>&			get_routes() const;
};