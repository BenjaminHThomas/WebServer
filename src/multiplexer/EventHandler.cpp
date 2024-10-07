/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:20:55 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/07 08:45:20 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <cstdio>
#include <exception>
#include <stdexcept>
#include <sys/socket.h>

class EventHandler::epollInitFailure : public std::exception {
	public:
		const char* what() const throw() {
			return "Error: could not initialise epoll\n";
		}
};

class EventHandler::epollWaitFailure : public std::exception {
	public:
		const char* what() const throw() {
			return "Error: epoll_wait failed\n";
		}
};

EventHandler::EventHandler(const Cluster &cluster) : _cluster(cluster)
{
	_epollFd = epoll_create1(0);
	if (_epollFd == -1) {
		throw epollInitFailure();
	}
}

EventHandler::~EventHandler()
{
	if (_epollFd != -1) {
		close(_epollFd);
	}
	std::map<int, ClientConnection*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it) {
		delete it->second;
	}
	_clients.clear();
}

void EventHandler::setNonBlock(int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		std::cerr << "Error: couldn't set fd to non-blocking\n";
		return ;
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		std::cerr << "Error: couldn't set fd to non-blocking\n";
	}
}

void EventHandler::changeToRead(int clientFd) {
	struct epoll_event ev;
	ev.data.fd = clientFd;
	ev.events = EPOLLIN;
	if (epoll_ctl(_epollFd, EPOLL_CTL_MOD, clientFd, &ev) == -1) {
		std::cerr << "Error: could not mark fd " << clientFd << " as EPOLLIN\n";
	}
}

void EventHandler::changeToWrite(int clientFd) {
	struct epoll_event ev;
	ev.data.fd = clientFd;
	ev.events = EPOLLOUT;
	if (epoll_ctl(_epollFd, EPOLL_CTL_MOD, clientFd, &ev) == -1) {
		std::cerr << "Error: could not mark fd " << clientFd << " as EPOLLOUT\n";
	}
}

bool EventHandler::addToEpoll(int fd) {
	setNonBlock(fd);
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		std::cerr << "Error: could not mark fd " << fd << " as EPOLLIN\n";
		return false;
	}
	return true;
}

bool EventHandler::deleteFromEpoll(int fd) {
	if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL) == -1) {
		std::cerr << "Error: could not delete fd " << fd << "\n";
		return false;
	}
	return true;
}

void EventHandler::addClient(int clientFd, const Config &config) {
	ClientConnection* conn = new ClientConnection(clientFd, config);
	_clients[clientFd] = conn;
	_openConns[clientFd] = EP_CLIENT;
}

void EventHandler::addServer(Server & s) {
	int serverFd = s.getSockFd();
	std::map<int, Server*>::iterator it = _servers.find(serverFd);
	if (it == _servers.end()) {
		if (!addToEpoll(serverFd)) {
			return ;
		}
		_servers[serverFd] = &s;
		_openConns[serverFd] = EP_SERVER;
	}
}

void EventHandler::handleNewConnection(Server & s) {
	int serverFd = s.getSockFd();
	std::cout << "New connection from " << serverFd << "\n";

	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	int clientFd = accept(serverFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
	if (clientFd == -1) {
		std::cerr << "Error: could not accept new connection from fd " << serverFd << "\n";
		return ;
	}
	if (!addToEpoll(clientFd)) {
		close(clientFd);
		return ;
	}
	addClient(clientFd, s.getConfig());
}

bool EventHandler::isMultiPartReq(int clientFd) {
	std::string req = _clients.at(clientFd)->_requestBuffer;
	return req.find("Content-Type: multipart") != std::string::npos;
}

bool EventHandler::isMultiPartReqFinished(int clientFd) {
	std::string req = _clients.at(clientFd)->_requestBuffer;
	std::string::size_type boundaryPos = req.find("boundary=");
	if (boundaryPos == std::string::npos) {
		return false;
	}
	std::string boundary = req.substr(boundaryPos + 9);
	boundary = boundary.substr(0, boundary.find("\r\n"));
	std::string boundaryFinish = boundary + "--";
	return req.find(boundaryFinish) != std::string::npos;
}

bool EventHandler::isResponseComplete(int clientFd) {
	// check header completion
	std::string buff = _clients.at(clientFd)->_requestBuffer;
	std::string::size_type headerEnd = getHeaderEndPos(clientFd);
	if (headerEnd == std::string::npos) {
		return false;
	}

	if (_clients.at(clientFd)->_reqType == (ClientConnection::reqType)CHUNKED || isHeaderChunked(clientFd)) {
		_clients.at(clientFd)->_reqType = (ClientConnection::reqType)CHUNKED;
		return isChunkReqFinished(clientFd);
	}

	if (isMultiPartReq(clientFd)) {
		return isMultiPartReqFinished(clientFd);
	}

	size_t content_len_pos = buff.find("Content-Length: ");
	if (content_len_pos != std::string::npos) {
		size_t content_len_end = buff.find("\r\n", content_len_pos);
		std::string content_len_str = buff.substr(content_len_pos + 16, content_len_end - (content_len_pos + 16));
		int content_length = std::atoi(content_len_str.c_str());
		return buff.length() >= (headerEnd + content_length);
	}
	return true;
}

std::string::size_type EventHandler::getHeaderEndPos(int clientFd) {
	std::string reqBuffer = _clients.at(clientFd)->_requestBuffer;
	std::string::size_type headerEnd = reqBuffer.find("\r\n\r\n");
	if (headerEnd == std::string::npos) {
		headerEnd = reqBuffer.find("\n\n");
		if (headerEnd == std::string::npos) {
			return std::string::npos;
		}
		headerEnd += 2;
	} else {
		headerEnd += 4;
	}
	return headerEnd;
}

bool EventHandler::isBodyTooBig(int clientFd) {
	uint64_t maxBodySize = _clients.at(clientFd)->_config.get_max_body_size();
	if (maxBodySize == 0)
		return false;
	std::string::size_type headerEndPos = getHeaderEndPos(clientFd);
	if (headerEndPos == std::string::npos) {
		return false;
	}
	std::string::size_type currentBodySize;
	currentBodySize = _clients.at(clientFd)->_requestBuffer.size() - headerEndPos;
	std::cout << "Body size: " << currentBodySize << "\n\n";
	return currentBodySize > maxBodySize;
}

void EventHandler::sendInvalidResponse(int clientFd) {
	std::cerr << "Error: body size limit reached.\n";
	//_clients.at(clientFd)->resetData();
	// change request buffer to cause 504
	_clients.at(clientFd)->_errorCode = 413;
	generateResponse(clientFd);
	changeToWrite(clientFd);
}

// Read all data from the client
void EventHandler::handleClientRequest(int clientFd) {
	char buffer[BUFFER_SIZE];
	int bytes_read;

	std::cout << "Handling client request from " << clientFd << "\n";
	bytes_read = read(clientFd, buffer, sizeof(buffer) - 1);
	if (bytes_read <= 0) {
		std::cerr << "Error: failed to read or client closed connection.\n";
		remove_client(clientFd);
		return ;
	}
	_clients.at(clientFd)->_requestBuffer.append(buffer, bytes_read);
	if (isBodyTooBig(clientFd)) {
		sendInvalidResponse(clientFd);
		return ;
	}
	if (isResponseComplete(clientFd))
	{
		if (_clients[clientFd]->_reqType == (ClientConnection::reqType)CHUNKED) {
			cleanChunkedReq(clientFd);
		}
		// Create a temporary Request object to check if CGI needs to be triggered
		Request	tmp_request(_clients.at(clientFd)->_requestBuffer);

		tmp_request.printAll();

		// Get corresponding Config and Route based on Client Request
		const Config &conf = get_config(tmp_request.getHeaderValue("Host"), clientFd);
		const Config::Routes &route = Response::find_match(conf, tmp_request.getUrl());

		std::map<std::string, std::string>::const_iterator cgi_route;
		if ((cgi_route = Response::check_cgi(route, tmp_request.getUrl())) != route.cgi.end())
		{
			_clients.at(clientFd)->_cgi = true;
			std::vector<std::string> arguments;
			arguments.push_back(cgi_route->second);
			std::string file;
			if (tmp_request.getUrl() == route.path)
				file = route.index;
			else
				file = tmp_request.getUrl().substr(route.path.length());
			arguments.push_back(route.directory + file);
			_clients.at(clientFd)->_cgiResult = startCGI(clientFd, arguments);
			if (_clients.at(clientFd)->_cgiResult != SUCCESS) {
				generateResponse(clientFd);
				changeToWrite(clientFd);
			}
		}
		else
		{
			generateResponse(clientFd);
			changeToWrite(clientFd);
		}
	}
}

// Write response to the client

void EventHandler::generateResponse(int clientFd) {
	Request	rqs(_clients.at(clientFd)->_requestBuffer);
	const Config &conf = get_config(rqs.getHeaderValue("Host"), clientFd);
	if (_clients.at(clientFd)->_errorCode)
	{
		Response rsp(conf, _clients.at(clientFd)->_errorCode);
		_clients.at(clientFd)->_responseBuffer.append(rsp.generateResponse());
	}
	else
	{
		std::string s;
		if (_clients.at(clientFd)->_cgi)
		{
			Response rsp(rqs, conf, _clients.at(clientFd)->_cgiBuffer, _clients.at(clientFd)->_cgiResult);
			s = rsp.generateResponse();
		}
		else
		{
			Response rsp(rqs, conf);
			s = rsp.generateResponse();
		}
		_clients.at(clientFd)->_responseBuffer.append(s);
	}
}

void EventHandler::remove_client(int clientFd)
{
	deleteFromEpoll(clientFd);
	delete _clients.at(clientFd);
	_openConns.erase(clientFd);
	_clients.erase(clientFd);
}

void EventHandler::handleResponse(int clientFd) {
	std::cout << "Sending response to client " << clientFd << "\n";
	ssize_t bytes_remaining = _clients.at(clientFd)->_responseBuffer.size();
	ssize_t bytes_written = write(clientFd, _clients.at(clientFd)->_responseBuffer.c_str(), _clients.at(clientFd)->_responseBuffer.length());
	if (bytes_written < 0) {
		std::cerr << "CHECK HERE FOR SURE IF YOU EVER GET THIS DURING DEBUG" << std::endl;
		remove_client(clientFd);
		return  ; // could be temporary full socket, but not allowed to check err code to confirm :(
	} else if (bytes_written == bytes_remaining) {
		if (_clients.at(clientFd)->_errorCode == 413)
			remove_client(clientFd);
		else
		{
			_clients.at(clientFd)->resetData();
			changeToRead(clientFd);
		}
	} else {
		_clients.at(clientFd)->_responseBuffer.erase(0, bytes_written);
	}
}

void EventHandler::checkCompleteCGIProcesses(void)
{
	std::map<int, CGIInfo*>::iterator it;
	std::vector<int> completed;
	std::vector<int> clients_complete;

	// for (std::map<int, ClientConnection *>::iterator it = _clients.begin(); it != _clients.end(); it ++)
	// {
	// 	if (write(it->second->_clientFd, 0, 0) != 0)
	// 		clients_complete.push_back(it->second->_clientFd);
	// }

	// for (std::vector<int>::const_iterator it = clients_complete.begin(); it < clients_complete.end(); it++)
	// 	remove_client(*it);

	for (it = _cgiManager._cgiProcesses.begin();
			it != _cgiManager._cgiProcesses.end();
			++it) {
		if (it->second->isFinished) {
			CGIInfo *info = it->second;
			int clientFd = info->clientFd;
			_clients.at(clientFd)->_cgiBuffer = info->output;
			deleteFromEpoll(info->pipeFd);
			completed.push_back(info->pipeFd);
			_openConns.erase(info->pipeFd);
			generateResponse(clientFd);
			changeToWrite(clientFd);
		}
	}

	for (std::vector<int>::const_iterator it = completed.begin(); it < completed.end(); it++)
		_cgiManager.deleteFromCGI(*it);
}

void EventHandler::epollLoop(void) {
	struct epoll_event eventQueue[MAX_EVENTS];

	while (1) {
		int numEvents = epoll_wait(_epollFd, eventQueue, MAX_EVENTS, 0);
		if (numEvents == -1) {
			throw epollWaitFailure();
		}
		for (int i = 0; i < numEvents; ++i) {
			int fd = eventQueue[i].data.fd;
			switch (_openConns[fd]) {
				case EP_SERVER:
					handleNewConnection(*_servers[fd]);
					break ;
				case EP_CGI:
					_cgiManager.readCGIOutput(fd);
					break ;
				case EP_CLIENT:
					if (eventQueue[i].events & EPOLLIN)
						handleClientRequest(fd);
					else if (eventQueue[i].events & EPOLLOUT)
						handleResponse(fd);
					break ;
			}
		}
		checkCompleteCGIProcesses();
	}
}

const Config &EventHandler::get_config(const std::string &host, int clientFd) const
{
	const std::vector<Config*>::const_iterator found = _cluster.get_config_by_host(host);
	if (_cluster.get_configs().end() == found)
		return _clients.at(clientFd)->_config;
	return **found;
}