/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 18:36:13 by tsuchen           #+#    #+#             */
/*   Updated: 2024/10/02 14:28:53 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Request.hpp"
# include "config.hpp"
# include <sstream>
# include <ctime>

class Response
{
private:
	/* data */
	std::string		_content;
	int				_statusCode;
	std::string		_contentType;
	Config::Routes	_route;
	std::string		getContent();
	
	/* init utils */
	int				initResponse(Request const &request);
	Config::Routes	const &find_match(std::string const &url, std::vector<Config::Routes> const &routes);
	
	/* static functions */
	static std::string	getCurrentTime();
	static const std::map<int, std::string>	_statusCodes;
public:
	Response(Request const &request, const std::vector<Config::Routes> &routes);
	~Response();

	std::string		generateResponse();
	static std::map<int, std::string>	initStatusCodes();
};


#endif