/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 18:52:17 by tsuchen           #+#    #+#             */
/*   Updated: 2024/10/02 11:56:42 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(Request const &request) : _statusCode(200), _contentType("text/html") {
	if (request.getUrl() == "/") {
		_content = "<html><body><h1>Welcome to My C++ Web Server!</h1></body></html>";
	} else if (request.getUrl() == "/about") {
		_content = "<html><body><h1>About Us</h1><p>This is a simple C++ web server.</p></body></html>";
	} else {
		_content = "<html><body><h1>404 Not Found</h1></body></html>";
		_statusCode = 404;
	}
}

Response::~Response() {}

int	Response::initResponse(Request const &request) {
	//find the last matched route and save it
	//const Config::Routes &route = find_match(request.getURL(), config.get_routes());
	
	// Check for allowed Methods
	//if (!std::count(route.methods.begin(), route.methods.end(), request.getMethod()))
	//		return 405;
	
	// Check for CGI
	//if (check_cgi(route.cgi, request.getURL()) == true)
	//		//trigger CGI loop to get content;
	
	return 200;
}


std::string Response::getCurrentTime() {
	time_t  now = time(0);
	struct tm t_struct;
	char    buff[80];
	t_struct = *gmtime(&now);
	strftime(buff, sizeof(buff), "%a, %d %b %Y %H:%M:%S GMT", &t_struct);
	return std::string(buff);
}

std::string Response::generateResponse() {
	std::ostringstream  response;
	
	response << "HTTP/1.1 " << _statusCodes.at(_statusCode) << "\r\n";
	response << "ContentType: " << _contentType << "\r\n";
	response << "Content-Length: " << _content.length() << "\r\n";
	response << "Date: " << getCurrentTime() << "\r\n";
	response << "Server: 3GoatServer/1.0\r\n";
	response << "\r\n";
	response << _content;

	return response.str();
}

std::map<int, std::string> Response::initStatusCodes() {
	std::map<int, std::string>	tmp;
	tmp[200] = "200 OK";
	tmp[201] = "201 Created";
	tmp[202] = "202 Accepted";
	tmp[204] = "204 No Content";
	tmp[300] = "300 Multiple Choices";
	tmp[301] = "301 Moved Permanently";
	tmp[302] = "302 Found";
	tmp[303] = "303 See Other";
	tmp[400] = "400 Bad Request";
	tmp[401] = "401 Unauthorized";
	tmp[403] = "403 Forbidden";
	tmp[404] = "404 Not Found";
	tmp[405] = "405 Method Not Allowed";
	tmp[418] = "418 I'm a teapot";
	tmp[502] = "502 Bad Gateway";
	tmp[504] = "504 Gateway Timeout";
	tmp[505] = "505 HTTP Version Not Supported";
}

const std::map<int, std::string> Response::_statusCodes = Response::initStatusCodes();