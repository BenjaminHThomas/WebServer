/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 15:11:44 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/24 08:46:39okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json.hpp"
#include "parser.hpp"
#include <temp.hpp>


int main(int ac, char **av)
{
	if (ac != 2)
		return 1;

	std::ifstream input(av[1]);
	if (!input)
		return 1;
	JsonValue json  = JSONParser::parse(input);

	std::cout << json << std::endl;

	return 0;
}
