/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 10:21:42 by tsuchen           #+#    #+#             */
/*   Updated: 2024/09/25 11:19:26 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() :
	_method(""), _url(""), _http_version(""), _body(""), _raw("")
{
	std::cout << "Default Request object constructed" << std::endl;
}

Request::Request(std::string const &str) : 
	_method(""), _url(""), _http_version(""), _body(""), _raw(str)
{
	std::cout << "String Request object construced" << std::endl;
	this->setMethod();
	this->setUrl();
	this->setHttpVersion();
	this->setHeaders();
	this->setBody();
}

Request::Request(const Request &other) :
	_method(""), _url(""), _http_version(""), _body(""), _raw("")
{
	*this = other;
}

Request&	Request::operator=(const Request &other) {
	if (this != &other) {
		this->_method = other._method;
		this->_url = other._url;
		this->_http_version = other._http_version;
		this->_headers = other._headers;
		this->_body = other._body;
		this->_raw = other._raw;
	}
	return *this;
}

Request::~Request() {
	std::cout << "A Request object is destroyed" << std::endl;
}

std::string const &Request::getMethod() const{
	return this->_method;
}

std::string const &Request::getUrl() const{
	return this->_url;
}

std::string const &Request::getHttpVersion() const{
	return this->_http_version;
}

std::string const &Request::getHeaderValue(std::string const &key) const{
	std::map<std::string, std::string>::const_iterator it = this->_headers.find(key);
	if (it != this->_headers.end()) {
		return it->second;
	}
	throw std::runtime_error("Key not found in _headers");
}

std::string const &Request::getBody() const {
	return this->_body;
}

void	Request::setMethod() {
	
}

void	Request::setUrl() {

}

void	Request::setHttpVersion() {
	
}

void	Request::setHeaders() {
	
}

void	Request::setBody() {
	
}
