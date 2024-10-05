/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnection.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:53:42 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/05 18:38:17 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CLIENTCONNECTION_HPP
# define CLIENTCONNECTION_HPP

# include "EventHandler.hpp"
# include <utils.hpp>


class ClientConnection
{
	public:
		enum reqType {
			CHUNKED,
			NONCHUNKED
		};

	public:
		int _clientFd;
		std::string _requestBuffer;
		std::string _responseBuffer;
		std::string _cgiBuffer;
		size_t	_bytesSent;
		bool _responseReady;
		CgiResult _cgiResult;
		bool _cgi;
		reqType _reqType;

		const Config &_config;

	public:
		void resetData();
		void closeConnection();

	public:
		ClientConnection (int fd, const Config &config);
		ClientConnection(const ClientConnection&);
		ClientConnection& operator=(const ClientConnection& other);
		~ClientConnection ();
};

#endif /* CLIENTCONNECTION_HPP */
