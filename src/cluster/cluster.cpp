/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 08:42:01 by okoca             #+#    #+#             */
/*   Updated: 2024/10/04 13:44:23 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cluster.hpp"
#include "EventHandler.hpp"
#include "Server.hpp"
#include "config.hpp"
#include "json.hpp"
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
	catch (const JsonValue::BadType &e)
	{
		throw Config::BadValue("bad config");
	}
	catch (const Config::BadValue &e)
	{
		throw Config::BadValue("bad config");
	}
	catch (const std::out_of_range &e)
	{
		throw Config::BadValue("config error, missing field");
	}
	catch (const std::exception &e)
	{
		std::cerr << "CLUSTER ERROR: " << e.what() << std::endl;
		throw e;
	}
}

Cluster &Cluster::start()
{
	EventHandler e;
	for (std::vector<Server*>::iterator it = _servers.begin(); it < _servers.end(); it++)
	{
		e.addServer(*(*it));
	}
	e.epollLoop();
	return *this;
}

Cluster::~Cluster()
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

const std::vector<Config*> &Cluster::get_configs() const
{
	return _configs;
}

const Config& Cluster::get_config_by_host(const std::string &host) const
{
	for (std::vector<Config*>::const_iterator it = _configs.begin(); it < _configs.end(); it++)
	{
		if (host == (*it)->get_host())
			return *(*it);
	}
	return *_configs.front();
}
