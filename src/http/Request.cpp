/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 10:21:42 by tsuchen           #+#    #+#             */
/*   Updated: 2024/10/02 09:54:48 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(std::string const &str) : AHttpData(str) {
	try
	{
		this->parseHead();
	}
	catch(const std::exception& e)
	{
		std::cerr << "[Request] Error: " << e.what() << std::endl;
	}
}

Request::Request(const Request &other) : AHttpData(other.getRaw()) {
	*this = other;
}

Request&	Request::operator=(const Request &other) {
	if (this != &other) {
		AHttpData::operator=(other);
		this->_method = other._method;
		this->_url = other._url;
		this->_http_version = other._http_version;
	}
	return *this;
}

Request::~Request() {}

std::string const &Request::getMethod() const{
	return this->_method;
}

std::string const &Request::getUrl() const{
	return this->_url;
}

std::string const &Request::getHttpVersion() const{
	return this->_http_version;
}

void	Request::parseHead() {
	/* Parse Method */
	std::size_t	mtd_end = _raw.find(" ");
	if (mtd_end == std::string::npos)
		throw std::runtime_error("Method Not Found");
	_method = _raw.substr(0, mtd_end);
	/* Parse URL */
	std::size_t	url_start = _raw.find_first_not_of(" ", mtd_end + 1);
	if (url_start == std::string::npos)
		throw std::runtime_error("URL Not Found");
	std::size_t url_end = _raw.find(" ", url_start);
	if (url_end == std::string::npos)
		throw std::runtime_error("URL Not Found");
	_url = _raw.substr(url_start, url_end - url_start);
	/* Parse HTTP version */
	std::size_t http_start = _raw.find_first_of("HTTP/", url_end + 1);
	if (http_start == std::string::npos)
		throw std::runtime_error("HTTP version Not Found");
	std::size_t http_end = _raw.find('\n', http_start);
	if (http_end == std::string::npos)
		throw std::runtime_error("HTTP version Not Found");
	_http_version = _raw.substr(http_start, http_end - http_start);
	if (!_http_version.empty() && _http_version[_http_version.length() - 1] == '\r')
		_http_version.erase(_http_version.length() - 1);
}

// Debug function to print all elements in Request class
void	Request::printAll() {
	std::cout << "Method:\t" << this->getMethod() << std::endl;
	std::cout << "Url:\t" << this->getUrl() << std::endl;
	std::cout << "Http:\t" << this->getHttpVersion() << std::endl;
	std::cout << "\n------------------Headers------------------" << std::endl;
	std::map<std::string, std::string>::iterator it;
	for (it = this->_headers.begin(); it != this->_headers.end(); it++) {
		std::cout << "[" << it->first << "] : \"" << it->second << "\"" << std::endl;
	}
	std::cout << "\n---------------End of Headers---------------" << std::endl;
	std::cout << "\n-------------------Body---------------------" << std::endl;
	std::cout << this->getBody() << std::endl;
	std::cout << "\n--------------------Raw---------------------" << std::endl;
	std::cout << this->_raw << std::endl;
}

/* This part could be deleted after having merged with Config classes */
// std::vector<std::string>	Request::initMethods() {
// 	std::vector<std::string>	methods;
	
// 	methods.push_back("GET");
// 	methods.push_back("HEAD");
// 	methods.push_back("POST");
// 	methods.push_back("PUT");
// 	methods.push_back("DELETE");
// 	methods.push_back("CONNECT");
// 	methods.push_back("OPTIONS");
// 	methods.push_back("TRACE");
// 	methods.push_back("PATCH");

// 	return methods;
// }

// std::vector<std::string>	Request::_allowdMethods = Request::initMethods();

/* The following part is separated to Abstract class AHTTP */
// std::string const &Request::getHeaderValue(std::string const &key) const{
// 	std::map<std::string, std::string>::const_iterator it = this->_headers.find(key);
// 	if (it != this->_headers.end()) {
// 		return it->second;
// 	}
// 	throw std::runtime_error("Key not found in _headers");
// }

// std::string const &Request::getBody() const {
// 	return this->_body;
// }

// void	Request::parseRest() {
// 	/* Parse Headers (key, value)*/
// 	std::string::const_iterator	it = _raw.begin();
// 	std::string::const_iterator end = _raw.end();
// 	it = std::find(it, end, '\n');	// first move to the end of first line
// 	if (it == end) return ;
// 	++it;
	
// 	while (it != end) {
// 		std::string::const_iterator	line_end = std::find(it, end, '\n');	// the end of next line
// 		if (line_end == it || (line_end != end && *(line_end - 1) == '\r' && line_end - 1 == it))
// 			break ;
// 		std::string::const_iterator colon = std::find(it, line_end, ':');
// 		if (colon == line_end) {
// 			// invalid header format, skip and continue
// 			it = line_end + 1;
// 			continue ;
// 		}
// 		std::string key(it, colon);
// 		std::string value(colon + 1, line_end);
// 		key = trim(key);
// 		value = trim(value);
// 		if (!value.empty() && value[value.length() - 1] == '\r')
// 			value.erase(value.length() - 1);
// 		if (!key.empty())
// 			_headers[key] = value;
// 		// std::cout << "Key: [" << key << "] | Value: [" << value << "]" << std::endl; 
// 		it = line_end + 1;
// 	}
// 	/* Parese Body if there is any*/
// 	if (it != end && std::find(it, end, '\n') != end)
// 		_body.assign(std::find(it, end, '\n') + 1, end);
// }

// std::string	Request::trim(std::string const & str) {
// 	std::string::const_iterator	start = std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int, int>(isspace)));
// 	std::string::const_iterator	end = std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int, int>(isspace))).base();
// 	return start < end ? std::string(start, end) : std::string();
// }
