/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 09:35:47 by okoca             #+#    #+#             */
/*   Updated: 2024/10/05 09:38:15 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sys/stat.h>
#include <utils.hpp>

bool	is_directory(const std::string &path)
{
	struct stat s;

	if (stat(path.c_str() ,&s) == 0)
	{
		if( s.st_mode & S_IFDIR )
			return true;
	}
	return false;
}

bool end_with_json(const std::string &c)
{
	std::string suffix = ".json";

	if (c.length() >= suffix.length())
	{
		return (c.compare(c.length() - suffix.length(), suffix.length(), suffix) == 0);
	}
	return false;
}

