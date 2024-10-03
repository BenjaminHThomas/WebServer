/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 10:21:42 by tsuchen           #+#    #+#             */
/*   Updated: 2024/10/03 18:22:05 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(std::string const &str) : AHttpData(str, true) {
	try
	{
		this->parseHead();
	}
	catch(const std::exception& e)
	{
		std::cerr << "[Request] Error: " << e.what() << std::endl;
	}
}

Request::Request(const Request &other) : AHttpData(other.getRaw(), true) {
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
