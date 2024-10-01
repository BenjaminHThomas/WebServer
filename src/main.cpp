/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 15:11:44 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/01 12:03:36 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"
#include "Request.hpp"

#include "json.hpp"
#include "parser.hpp"
#include <temp.hpp>


int main(int ac, char **av, char **env)
{
	if (ac != 2)
		return 1;

	std::ifstream input(av[1]);
	if (!input)
		return 1;
	JsonValue json  = JSONParser::parse(input);

	std::cout << json << std::endl;

	try {
		Server s;
		EventHandler e(av, env);
		if (!e.addToEpoll(s.getSockFd()))
			return 1;
		e.epollLoop(s);
	} catch (std::exception &e) {
		std::cerr << e.what();
	}

	return 0;
}
/* Testing for Reuqest Parsing */
	// std::string	raw = "GET /login HTTP/1.1\r\nHost: www.example.com\r\nConnection: keep-alive\r\nContent-Length: 29\r\nCache-Control: max-age=0\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36\r\nContent-Type: application/x-www-form-urlencoded\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: en-US,en;q=0.9\r\n\r\nusername=johndoe&password=12345";
	// // std::string	raw = "POST /login HTTP/1.1\nHost: www.example.com\nConnection: keep-alive\nContent-Length: 29\nCache-Control: max-age=0\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36\nContent-Type: application/x-www-form-urlencoded\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8\nAccept-Encoding: gzip, deflate, br\nAccept-Language: en-US,en;q=0.9\n\nusername=johndoe&password=12345";
	// Request r(raw);
	// Request	rrr(r);
	// rrr.printAll();