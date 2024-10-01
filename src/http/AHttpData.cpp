/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpData.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 19:03:45 by tsuchen           #+#    #+#             */
/*   Updated: 2024/09/30 14:22:07 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AHttpData.hpp"

AHttpData::AHttpData(std::string const & str) : _raw(str) {
	// std::cout << "HttpData constructed" << std::endl;
	try
	{
		/* code */
		this->parseRest();
	}
	catch(const std::exception& e)
	{
		std::cerr << "[HttpData] Error: " << e.what() << std::endl;
	}
}

AHttpData::AHttpData(AHttpData const & other) : _raw(other._raw) {
	// std::cout << "HttpData object copied" << std::endl;
	*this = other;
}

AHttpData&	AHttpData::operator=(AHttpData const & other) {
	if (this != &other) {
		this->_body = other._body;
		this->_headers = other._headers;
	}
	return *this;
}

AHttpData::~AHttpData() {
	// std::cout << "An HttpData is destroyed" << std::endl;
}

std::string const &AHttpData::getHeaderValue(std::string const &key) const{
	std::map<std::string, std::string>::const_iterator it = this->_headers.find(key);
	if (it != this->_headers.end()) {
		return it->second;
	}
	throw std::runtime_error("Key not found in _headers");
}

std::string const &AHttpData::getBody() const {
	return this->_body;
}

std::string const &AHttpData::getRaw() const {
	return this->_raw;
}

void	AHttpData::parseRest() {
	/* Parse Headers (key, value)*/
	std::string::const_iterator	it = _raw.begin();
	std::string::const_iterator end = _raw.end();
	it = std::find(it, end, '\n');	// first move to the end of first line
	if (it == end) return ;
	++it;
	
	while (it != end) {
		std::string::const_iterator	line_end = std::find(it, end, '\n');	// the end of next line
		if (line_end == it || (line_end != end && *(line_end - 1) == '\r' && line_end - 1 == it))
			break ;
		std::string::const_iterator colon = std::find(it, line_end, ':');
		if (colon == line_end) {
			// invalid header format, skip and continue
			it = line_end + 1;
			continue ;
		}
		std::string key(it, colon);
		std::string value(colon + 1, line_end);
		key = trim(key);
		value = trim(value);
		if (!value.empty() && value[value.length() - 1] == '\r')
			value.erase(value.length() - 1);
		if (!key.empty())
			_headers[key] = value;
		// std::cout << "Key: [" << key << "] | Value: [" << value << "]" << std::endl; 
		it = line_end + 1;
	}
	/* Parese Body if there is any*/
	if (it != end && std::find(it, end, '\n') != end)
		_body.assign(std::find(it, end, '\n') + 1, end);
}

std::string AHttpData::trim(std::string const & str) {
	std::string::const_iterator	start = std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int, int>(isspace)));
	std::string::const_iterator	end = std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int, int>(isspace))).base();
	return start < end ? std::string(start, end) : std::string();
}
