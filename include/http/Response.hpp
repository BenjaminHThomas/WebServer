/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 18:36:13 by tsuchen           #+#    #+#             */
/*   Updated: 2024/10/07 10:30:19 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Request.hpp"
# include "config.hpp"
# include "EventHandler.hpp"
# include "ClientConnection.hpp"
# include <ctime>

class Response
{
public:
	enum	TimeForm {
		STANDARD,
		SIMPLE,
	};

private:
	/* data */
	std::string							_content;
	int									_statusCode;
	std::string							_contentType;
	Config::Routes						_route;
	std::map<std::string, std::string>	_extraHeaders;
	Config const 	&_config;

	/* init utils */
	std::string		readFile(const std::string &filename);
	std::string		getFileContent(std::string const &url);
	std::string		getPostContent(Request const &request);

	/* static functions */
	static std::string	toLower(std::string s);
	static std::string	getCurrentTime(Response::TimeForm mode);

	/* class level const data set */
	static const std::map<int, std::string>	_statusCodes;
	static const std::vector<std::string> _allowedCGI;
	static const std::map<std::string, std::string>	_acceptedFile;
	static const std::map<std::string, std::string> _acceptedFileReversed;
public:
	Response(Config const &config, int errCode);
	Response(Request const &request, Config const &config);
	Response(Request const &request, Config const &config, const std::string &cgi_content, CgiResult cgi_res);
	~Response();

	std::string		generateResponse();
	/* static data set init */
	static const std::map<int, std::string>	initStatusCodes();
	static const std::vector<std::string> init_allowed_cgi();
	static const std::map<std::string, std::string>	initAcceptedFile();
	static const std::map<std::string, std::string> initAcceptedFileReversed();
	static bool						check_extension(std::string const &url);
	static Config::Routes const &	find_match(const Config &config, std::string const &url);
	static std::map<std::string, std::string>::const_iterator	check_cgi(const Config::Routes &route, std::string const &url);
	static std::string	check_postFile(std::string const &type);
	std::string		getErrorContent(int errCode);
	/* Post */

	/* DIRECTORY LISTING */
private:
	static const std::string _html_template;

	std::string directory_listing(const std::string &path, const std::string &file);
};


#endif