/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 18:36:13 by tsuchen           #+#    #+#             */
/*   Updated: 2024/10/01 15:03:45 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Request.hpp"
# include <sstream>
# include <ctime>

class Response
{
private:
	/* data */
	std::string		_content;
	std::string		_statusCode;
	std::string		_contentType;
	std::string		getContent();
	static std::string	getCurrentTime();
public:
	Response(Request const &request);
	~Response();

	std::string		generateResponse();
};


#endif