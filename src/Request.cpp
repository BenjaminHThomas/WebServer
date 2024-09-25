/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 10:21:42 by tsuchen           #+#    #+#             */
/*   Updated: 2024/09/25 13:25:19 by tsuchen          ###   ########.fr       */
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
	try
	{
		/* code */
		this->setMethod();
		this->setUrl();
		this->setHttpVersion();
		this->setHeaders();
		this->setBody();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
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
	std::size_t	del = _raw.find_first_of(" ");
	std::string	mtd = _raw.substr(0, del);
	if (std::count(_allowdMethods.begin(), _allowdMethods.end(), mtd))
		this->_method = mtd;
	else
		throw std::runtime_error("Not allowed Method");
	// std::string	url = _raw.substr(del + 1, _raw.find("HTTP/") - (del + 1));
	// this->_url = url;
}

void	Request::setUrl() {
	std::size_t del = _raw.find("HTTP/");
	std::size_t start = _raw.find_first_of('/');
	std::string url = _raw.substr(start, del - start);
	this->_url = url;
}

void	Request::setHttpVersion() {
	std::size_t del = _raw.find("\r\n");
	std::size_t start = _raw.find("HTTP/");
	std::string http = _raw.substr(start, del - start);
	this->_http_version = http;
}

void	Request::setHeaders() {
	
}

void	Request::setBody() {
	
}

std::vector<std::string>	Request::initMethods() {
	std::vector<std::string>	methods;
	
	methods.push_back("GET");
	methods.push_back("HEAD");
	methods.push_back("POST");
	methods.push_back("PUT");
	methods.push_back("DELETE");
	methods.push_back("CONNECT");
	methods.push_back("OPTIONS");
	methods.push_back("TRACE");
	methods.push_back("PATCH");

	return methods;
}

std::vector<std::string>	Request::_allowdMethods = Request::initMethods();

void	Request::printAll() {
	std::cout << "Method:\t" << this->getMethod() << std::endl;
	std::cout << "Url:\t" << this->getUrl() << std::endl;
	std::cout << "Http:\t" << this->getHttpVersion() << std::endl;
	std::cout << "\n------------------Headers------------------" << std::endl;
	std::map<std::string, std::string>::iterator it;
	for (it = this->_headers.begin(); it != this->_headers.end(); it++) {
		std::cout << it->first << " : " << it->second << std::endl;
	}
	std::cout << "\n---------------End of Headers---------------" << std::endl;
	std::cout << "\n-------------------Body---------------------" << std::endl;
	std::cout << this->getBody() << std::endl;
	std::cout << "\n--------------------Raw---------------------" << std::endl;
	std::cout << this->_raw << std::endl;
}