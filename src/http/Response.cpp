/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 18:52:17 by tsuchen           #+#    #+#             */
/*   Updated: 2024/10/02 16:59:21 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(Request const &request, Config const &config) : 
	_statusCode(200), _contentType("text/html"), _config(config)
{
	_route = find_match(request.getUrl());
	if (!std::count(_route.methods.begin(), _route.methods.end(), request.getMethod())) {
		_statusCode = 405;
		_content = getErrorContent(_statusCode);
	} else {
		
	}
	// if (request.getUrl() == "/") {
	// 	_content = "<html><body><h1>Welcome to My C++ Web Server!</h1></body></html>";
	// } else if (request.getUrl() == "/about") {
	// 	_content = "<html><body><h1>About Us</h1><p>This is a simple C++ web server.</p></body></html>";
	// } else {
	// 	_content = "<html><body><h1>404 Not Found</h1></body></html>";
	// 	_statusCode = 404;
	// }
}

Response::~Response() {}

int	Response::initResponse(Request const &request) {
	
	
	// Check for CGI
	//if (check_cgi(route.cgi, request.getURL()) == true)
	//		//trigger CGI loop to get content;
	
	return 200;
}

Config::Routes const & Response::find_match(std::string const &url) {
	std::vector<Config::Routes>::const_iterator found = _config.get_routes().begin();
	for (std::vector<Config::Routes>::const_iterator
	it = _config.get_routes().begin(); it != _config.get_routes().end(); ++it) {
		if (it->path.compare(0, it->path.length(), url) == 0) {
			if (url.at(it->path.length() + 1) == std::string::npos ||
				url.at(it->path.length() + 1) == '/') {
					found = it;
			}
		}
	}
	return *found;
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

std::string		Response::readFile(const std::string &filename) {
	std::ifstream	ifs(filename.c_str());
	if (!ifs) {
		throw std::runtime_error("Cannot open file: " + filename);
	}
	std::string		content;
	std::string		line;
	while (std::getline(ifs, line)) {
		content.append(line + '\n');
	}
	if (ifs.bad()) {
		throw std::runtime_error("Error occurred while reading the file: " + filename);
	}
	return content;
}

std::string		Response::getErrorContent(int errCode) {
	std::string content;
	if (_config.get_error_pages().size() != 0) {
		try
		{
			std::string const &err_page = _config.get_error_pages().at(errCode);
			content = readFile(err_page);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			content.append("<html><body><h1>");
			content.append(_statusCodes.at(_statusCode));
			content.append("</h1></body></html>");
		}
	}
	return content;
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