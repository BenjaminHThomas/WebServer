/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler_chunks.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 10:54:25 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/04 13:53:02 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"

/*
POST /upload HTTP/1.1
Host: example.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:100.0) Gecko/20100101 Firefox/100.0
Content-Type: text/plain
Transfer-Encoding: chunked\r\n\r\n
7\r\n
Mozilla\r\n
9\r\n
Developer\r\n
7\r\n
Network\r\n
0\r\n
\r\n

0 size chunk indicates the end;
double CRLF (or \n\n) signifies end of header;
*/

bool EventHandler::isHeaderChunked(int clientFd) {
	std::string request = _clients[clientFd]->_requestBuffer;
	return (request.find("Transfer-Encoding: chunked") != std::string::npos);
}

bool EventHandler::isChunkReqFinished(int clientFd) {
	std::string request = _clients[clientFd]->_requestBuffer;
	return (request.find("0\r\n\r\n") != std::string::npos);
}

std::string EventHandler::extractHeader(std::string & reqBuffer, std::string::size_type & headerEnd) {
	// extract header
	headerEnd = reqBuffer.find("\r\n\r\n");
	if (headerEnd == std::string::npos) {
		headerEnd = reqBuffer.find("\n\n");
		if (headerEnd == std::string::npos) {
			throw std::runtime_error("Invalid request: no header-body separator found\n");
		}
		headerEnd += 2;
	} else {
		headerEnd += 4;
	}
	return reqBuffer.substr(0, headerEnd);
}

std::string::size_type getChunkSize(std::string::size_type & chunkStart,
									std::string::size_type & chunkEnd,
									std::string & reqBuffer) {
	std::istringstream iss;
	std::string::size_type chunkSize;
	
	chunkEnd = reqBuffer.find("\r\n", chunkStart);
	if (chunkEnd == std::string::npos) {
		throw std::runtime_error("Invalid chunked encoding: chunk size not found\n");
	}
	iss.str(reqBuffer.substr(chunkStart, chunkEnd - chunkStart));
	iss.clear(); // clear err flags
	if (!(iss >> std::hex >> chunkSize)) {
		throw std::runtime_error("Invalid chunked encoding: Cannot parse chunk size");
	}
	return chunkSize;
}

void EventHandler::cleanChunkedReq(int clientFd) {
	std::string request = _clients[clientFd]->_requestBuffer;
	std::string decodedContent;
	std::string::size_type chunkSize;
	std::string::size_type headerEnd, chunkStart, chunkEnd;

	std::string header = extractHeader(request, headerEnd);

	// process chunks
	chunkStart = headerEnd;
	while (true) {
		// parse chunk size
		chunkSize = getChunkSize(chunkStart, chunkEnd, request);
		if (chunkSize == 0) {
			chunkStart = chunkEnd + 2;
			// check for trailing headers
			std::string trailingHeaders = request.substr(chunkStart);
			if (!trailingHeaders.empty() && trailingHeaders != "\r\n") {
				header += "\r\n" + trailingHeaders;
			}
			break ;
		}
		// extract content
		chunkStart = chunkEnd + 2; // skip CRLF
		if (chunkStart + chunkSize > request.length()) {
			throw std::runtime_error("Invalid chunked encoding: incomplete chunk data\n");
		}
		decodedContent += request.substr(chunkStart, chunkSize);
		chunkStart += chunkSize + 2;
	}
	// add content length to header;
	std::ostringstream contentLenHeader;
	contentLenHeader << "Content-Length: " << decodedContent.length() << "\r\n";
	header.insert(header.length() - 2, contentLenHeader.str());

	// remove transfer-encoding from header
	std::string::size_type  teStart = header.find("Transfer-Encoding:");
	if (teStart != std::string::npos) {
		std::string::size_type teEnd = header.find("\r\n", teStart);
		if (teEnd != std::string::npos) {
			header.erase(teStart, teEnd - teStart + 2);
		}
	}
	_clients[clientFd]->_requestBuffer = header + decodedContent;
}
