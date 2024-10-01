/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 18:52:17 by tsuchen           #+#    #+#             */
/*   Updated: 2024/10/01 14:37:15 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(Request const &request) {
    _statusCode = "200 OK";
    _contentType = "text/html";
    
    if (request.getUrl() == "/") {
        _content = "<html><body><h1>Welcome to My C++ Web Server!</h1></body></html>";
    } else if (request.getUrl() == "/about") {
        _content = "<html><body><h1>About Us</h1><p>This is a simple C++ web server.</p></body></html>";
    } else {
        _content = "<html><body><h1>404 Not Found</h1></body></html>";
        _statusCode = "404 Not Found";
    }
}

Response::~Response() {}

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
    
    response << "HTTP/1.1 " << _statusCode << "\r\n";
    response << "ContentType: " << _contentType << "\r\n";
    response << "Content-Length: " << _content.length() << "\r\n";
    response << "Date: " << getCurrentTime() << "\r\n";
    response << "Server: 3GoatServer/1.0\r\n";
    response << "\r\n";
    response << _content;

    return response.str();
}