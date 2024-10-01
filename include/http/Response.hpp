/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 18:36:13 by tsuchen           #+#    #+#             */
/*   Updated: 2024/09/30 18:51:37 by tsuchen          ###   ########.fr       */
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
	static std::string	getCurrentTime();
public:
	Response(Request const &request);
	~Response();

	std::string		generateResponse();
};


#endif