/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 08:42:01 by okoca             #+#    #+#             */
/*   Updated: 2024/10/04 21:36:50 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cluster.hpp"
#include "EventHandler.hpp"
#include "Server.hpp"
#include "config.hpp"
#include "json.hpp"
#include <sstream>
#include <stdexcept>
#include <vector>

Cluster::Cluster(const JsonValue &json)
{
	try
	{
		for (JsonValue::const_iter_arr it = json.begin_arr(); it < json.end_arr(); it++)
		{
			Config	*current = new Config(*it);
			_configs.push_back(current);
			_servers.push_back(new Server(*current));
		}
	}
	catch (const std::exception &e)
	{
		Cluster::clear();
		throw std::runtime_error(e.what());
	}
}

Cluster &Cluster::start()
{
	EventHandler e(*this);
	for (std::vector<Server*>::iterator it = _servers.begin(); it < _servers.end(); it++)
	{
		e.addServer(*(*it));
	}
	e.epollLoop();
	return *this;
}

void Cluster::clear()
{
	for (std::vector<Server*>::iterator it = _servers.begin(); it < _servers.end(); it++)
	{
		if (*it != NULL)
			delete *it;
	}
	for (std::vector<Config*>::iterator it = _configs.begin(); it < _configs.end(); it++)
	{
		if (*it != NULL)
			delete *it;
	}
}

Cluster::~Cluster()
{
	Cluster::clear();
}

const std::vector<Config*> &Cluster::get_configs() const
{
	return _configs;
}

// Returns an iterator of config, if it can't be found it returns `end()`
const std::vector<Config*>::const_iterator Cluster::get_config_by_host(const std::string &host) const
{
	std::stringstream s;
	for (std::vector<Config*>::const_iterator it = _configs.begin(); it < _configs.end(); it++)
	{
		s << (*it)->get_host() << ':' << (*it)->get_port();
		if (host == s.str())
		{
			std::cerr << "FOUND: [" << s.str() << "], actual host: [" << host << ']' << std::endl;
			return it;
		}
		s.str("");
	}
	return _configs.end();
}