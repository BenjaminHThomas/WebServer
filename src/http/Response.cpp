/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 18:52:17 by tsuchen           #+#    #+#             */
/*   Updated: 2024/10/05 09:37:31 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "CgiContent.hpp"
#include "utils.hpp"
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <vector>

Response::Response(Request const &request, Config const &config) :
	_statusCode(200), _contentType("text/html"), _config(config)
{
	_route = find_match(_config, request.getUrl());
	// check if method is allowed in the scope of _route
	if (!std::count(_route.methods.begin(), _route.methods.end(), request.getMethod())) {
		_statusCode = 405;
		_content = getErrorContent(_statusCode);
	}
	else if (request.getHttpVersion() != "HTTP/1.1")
	{
		_statusCode = 505;
		_content = getErrorContent(_statusCode);
	}
	else if (request.getMethod() == "POST")
	{
		// Post stuff here
		_content = getPostContent(request);
	}
	else if (check_extension(request.getUrl()))
	{
		_statusCode = 502;
		_content = getErrorContent(_statusCode);
	}
	else
	{
		_content = getFileContent(request.getUrl());
	}
}

Response::Response(Request const &request, Config const &config, const std::string &cgi_content, bool complete) :
	_statusCode(200), _contentType("text/html"), _config(config)
{
	_route = find_match(_config, request.getUrl());
	if (!std::count(_route.methods.begin(), _route.methods.end(), request.getMethod())) {
		_statusCode = 405;
		_content = getErrorContent(_statusCode);
	}
	else if (!complete)
	{
		_statusCode = 404;
		_content = getErrorContent(_statusCode);
	}
	else
	{
		CgiContent	cgi(cgi_content);
		_extraHeaders = cgi.getHeaders();
		_content = cgi.getBody();
	}
}

Response::~Response() {}

Config::Routes const & Response::find_match(const Config &config, std::string const &url) {
	std::vector<Config::Routes>::const_iterator found = config.get_routes().begin();

	for (std::vector<Config::Routes>::const_iterator
	it = config.get_routes().begin(); it != config.get_routes().end(); ++it) {
		if (url.find(it->path) == 0 && it->path.length() > found->path.length())
			found = it;
	}
	return *found;
}

std::string Response::generateResponse() {
	std::ostringstream  response;

	response << "HTTP/1.1 " << _statusCodes.at(_statusCode) << "\r\n";
	response << "Content-Type: " << _contentType << "\r\n";
	response << "Content-Length: " << _content.length() << "\r\n";
	response << "Date: " << getCurrentTime(STANDARD) << "\r\n";
	response << "Server: 3GoatServer/1.0\r\n";
	if (!_extraHeaders.empty()) {
		for (std::map<std::string, std::string>::iterator it = _extraHeaders.begin();
			it != _extraHeaders.end(); ++it) {
			response << it->first << ": " << it->second << "\r\n";
		}
	}
	response << "\r\n";
	response << _content;

	return response.str();
}

std::string		Response::readFile(const std::string &filename) {
	std::ifstream	ifs(filename.c_str());
	if (!ifs) {
		throw 404;
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
		catch(int)
		{
			std::cerr << "ERROR PAGES -> CATCHED INT" << '\n';
			content.append("<html><body>");
			content.append("<h2>Oops! Got an error: </h2><h1>");
			content.append(_statusCodes.at(_statusCode));
			content.append("</h1></body></html>");
		}
		catch(const std::exception& e)
		{
			std::cerr << "No Error pages: " << e.what() << '\n';
			content.append("<html><body>");
			content.append("<h2>Oops! Got an error: </h2><h1>");
			content.append(_statusCodes.at(_statusCode));
			content.append("</h1></body></html>");
		}
	}
	return content;
}

std::string		Response::getFileContent(std::string const &url) {
	std::string filename;
	std::string appended;
	std::string	file = url.substr(_route.path.length());

	filename = _route.directory;
	if (file != "/")
	{
		if (*file.begin() == '/' && *filename.rbegin() == '/')
			filename += file.substr(1);
		else
			filename += file;
	}
	std::string content;
	try
	{
		if (is_directory(filename))
		{
			std::cout << "-----------DIRECTORY---------" << std::endl;
			try
			{
				appended = filename;
				if (*appended.rbegin() != '/')
					appended += '/';
				appended += _route.index;
				content = readFile(appended);
			}
			catch (int)
			{
				if (!_route.dir_listing)
					throw 403;
				content = directory_listing(filename, url);
			}
			catch (const std::exception &e)
			{
				if (!_route.dir_listing)
					throw 403;
				content = directory_listing(filename, url);
			}
		}
		else
		{
			std::cout << "-----------REGULAR FILE---------" << std::endl;
			content = readFile(filename);
		}
	}
	catch(int status_code)
	{
		std::cerr << "Error: " << status_code << ", FILE: " << filename << ", appended: " << appended << '\n';
		_statusCode = status_code;
		content = getErrorContent(_statusCode);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error 500: " << e.what() << '\n';
		_statusCode = 500;
		content = getErrorContent(_statusCode);
	}
	return content;
}

std::string	Response::getPostContent(Request const &request) {
	// std::string	content;
	std::ostringstream content;
	std::string filename;
	std::string path = _route.upload;


	if (*path.rbegin() != '/')
		path.append("/");
	try
	{
		_contentType = request.getHeaderValue("Content-Type");	// throw outof range if no content_type in request
		filename = getCurrentTime(SIMPLE) + check_postFile(_contentType); // throw 403 if not allowed file format
		path += filename;
		std::ofstream ofs(path.c_str(), std::ofstream::out | std::ofstream::trunc);
		if (!ofs)
			throw (401); // not authorized to create a file
		ofs << request.getBody();
		_statusCode = 201;
		content << filename << " is created successfully at [" << _route.upload << "]\r\n";
	}
	catch(int statusCode)
	{
		_statusCode = statusCode;
		content << getErrorContent(_statusCode);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Other Post Error: " << e.what() << '\n';
		_statusCode = 400;
		content << getErrorContent(_statusCode);
	}
	return content.str();
}

// return the iterator of cgi found in the current _route
std::map<std::string, std::string>::const_iterator	Response::check_cgi(const Config::Routes &route, std::string const &url)
{
	if (route.cgi.empty())
		return route.cgi.end();
	std::string::size_type dotPos = url.rfind('.');
	if (dotPos == std::string::npos)
		return route.cgi.end();
	std::string	ext = toLower(url.substr(dotPos + 1));
	std::map<std::string, std::string>::const_iterator it = route.cgi.find(ext);
	return it;
}

std::string Response::check_postFile(std::string const &type)
{
	if (type.empty())
		// missing content-type header in the request
		throw (400); //Bad request
	if (_acceptedPostFile.count(type) > 0)
		return _acceptedPostFile.at(type);
	else
		throw (403); //Forbidden file type
}

bool	Response::check_extension(std::string const &url)
{
	if (url.empty())
		return false;
	std::string::size_type dotPos = url.rfind('.');
	if (dotPos == std::string::npos)
		return false;
	std::string	ext = toLower(url.substr(dotPos + 1));
	if (std::count(_allowedCGI.begin(), _allowedCGI.end(), ext) > 0)
		return true;
	return false;
}



std::string		Response::toLower(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}

std::string Response::getCurrentTime(Response::TimeForm mode) {
	time_t  now = time(0);
	struct tm t_struct;
	char    buff[80];
	t_struct = *gmtime(&now);
	if (mode == STANDARD)
		strftime(buff, sizeof(buff), "%a, %d %b %Y %H:%M:%S GMT", &t_struct);
	else
		strftime(buff, sizeof(buff), "%F_%H%M%S", &t_struct);
	return std::string(buff);
}

const std::map<int, std::string> Response::initStatusCodes() {
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
	tmp[500] = "500 Internal Server Error";
	tmp[502] = "502 Bad Gateway";
	tmp[504] = "504 Gateway Timeout";
	tmp[505] = "505 HTTP Version Not Supported";
	return tmp;
}

const std::vector<std::string> Response::init_allowed_cgi()
{
	std::vector<std::string> cgi;
	cgi.push_back("py");
	cgi.push_back("php");
	return cgi;
}

const std::map<std::string, std::string> Response::initAcceptedPostFile() {
	std::map<std::string, std::string>	tmp;
	tmp["application/octet-stream"] = ".bin";
	tmp["application/json"] = ".json";
	tmp["application/pdf"] = ".pdf";
	tmp["application/x-httpd-php"] = ".php";
	tmp["application/x-sh"] = ".sh";
	tmp["application/x-tar"] = ".tar";
	tmp["text/css"] = ".css";
	tmp["text/csv"] = ".csv";
	tmp["text/html"] = ".html";
	tmp["text/plain"] = ".txt";
	tmp["text/javascript"] = ".js";
	tmp["image/gif"] = ".gif";
	tmp["image/jpeg"] = ".jpeg";
	tmp["image/png"] = ".png";
	tmp["audio/mpeg"] = ".mp3";
	tmp["video/mp4"] = ".mp4";
	return tmp;
}

const std::map<int, std::string> Response::_statusCodes = Response::initStatusCodes();

const std::vector<std::string> Response::_allowedCGI = Response::init_allowed_cgi();

const std::map<std::string, std::string> Response::_acceptedPostFile = Response::initAcceptedPostFile();