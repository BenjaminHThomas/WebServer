/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 10:00:13 by tsuchen           #+#    #+#             */
/*   Updated: 2024/09/25 11:19:31 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <map>

class Request
{
private:
	/* data */
	std::string		_method;
	std::string		_url;
	std::string		_http_version;
	std::map<std::string, std::string>		_headers;
	std::string		_body;
	std::string		_raw;
public:
	/* constructors & destructors */
	Request();
	Request(std::string const &str);
	Request(const Request &other);
	virtual ~Request();
	/* overload operators */
	Request&	operator=(const Request &other);
	/* getters */
	std::string const &getMethod() const;
	std::string const &getUrl() const;
	std::string const &getHttpVersion() const;
	std::string const &getHeaderValue(std::string const &key) const;
	std::string const &getBody() const;
	/* setters */
	void	setMethod();
	void	setUrl();
	void	setHttpVersion();
	void	setHeaders();
	void	setBody();

	/* Utils */
};

#endif