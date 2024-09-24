/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:28:22 by okoca             #+#    #+#             */
/*   Updated: 2024/09/24 10:56:40 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <temp.hpp>

class JSONParser
{
	private:
	public:
		JSONParser(std::ifstream &stream);
		~JSONParser();
};