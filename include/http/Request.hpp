/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 10:00:13 by tsuchen           #+#    #+#             */
/*   Updated: 2024/10/05 21:12:03 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "AHttpData.hpp"

class Request : public AHttpData
{
private:
	/* data */
	std::string		_method;
	std::string		_url;
	std::string		_http_version;
	std::string		_query_string;
	/* static data */
	// static std::vector<std::string>	_allowdMethods;
public:
	/* constructors & destructors */
	Request(std::string const &str);
	Request(const Request &other);
	virtual ~Request();
	/* overload operators */
	Request&	operator=(const Request &other);
	/* getters */
	std::string const &getMethod() const;
	std::string const &getUrl() const;
	std::string const &getHttpVersion() const;
	std::string const &getQueryString() const;
	/* parser */
	void	parseHead();
	// void	parseRest();

	/* Utils */
	void	printAll();
	// static std::vector<std::string>	initMethods();
};

#endif