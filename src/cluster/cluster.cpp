/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 08:42:01 by okoca             #+#    #+#             */
/*   Updated: 2024/10/02 11:14:52 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cluster.hpp"
#include "EventHandler.hpp"
#include "Server.hpp"
#include <vector>

Cluster::Cluster(const JsonValue &json)
{
	try
	{
		for (JsonValue::const_iter_arr it = json.begin_arr(); it < json.end_arr(); it++)
		{
			Config cfg(*it);

			Server* srv = new Server(cfg);
			_servers.push_back(srv);
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "config error: " << e.what() << std::endl;
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
}
