/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpData.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:47:57 by tsuchen           #+#    #+#             */
/*   Updated: 2024/10/03 15:44:31 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef AHTTPDATA_HPP
# define AHTTPDATA_HPP

# include <iostream>
# include <algorithm>
# include <fstream>
# include <string>
# include <map>
# include <vector>

class AHttpData
{
private:
	/* data */
protected:
	std::map<std::string, std::string>	_headers;
	std::string		_body;
	bool			_hasHeaders;
	std::string	const	_raw;
	bool const		_isRequest;

public:
	AHttpData(std::string const &str, bool isRequest);
	AHttpData(AHttpData const &other);
	virtual ~AHttpData();
	/* overload operators */
	AHttpData&	operator=(AHttpData const &other);
	/* getters */
	std::string const &getHeaderValue(std::string const &key) const;
	std::map<std::string, std::string>	const &getHeaders() const;
	std::string const &getBody() const;
	std::string const &getRaw() const;
	/* parser */
	virtual void	parseHead() = 0;
	bool			parseHeaders();
	/* utils */
	static std::string	trim(std::string const & str);
};


#endif