/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 15:11:44 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/23 22:01:08 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <temp.hpp>

std::string handle_string(std::string::iterator &begin, const std::string::iterator &end)
{
	std::string s;
	while (begin++ < end)
	{
		if (*begin == '"')
			break;
		s.push_back(*begin);
	}
	return s;
}

int main(int ac, char **av)
{
	if (ac != 2)
		return 1;

	std::ifstream input(av[1]);
	if (!input)
		return 1;

	std::string	_data;
	std::string	_buf;
	while (std::getline(input, _buf))
	{
		std::string::iterator _c = _buf.begin();

		for (; _c < _buf.end(); _c++)
		{
			switch (*_c)
			{
				case '{':
					std::cout << "LCURLY" << std::endl;
					break;
				case '}':
					std::cout << "RCURLY" << std::endl;
					break;
				case ':':
					std::cout << "COLON" << std::endl;
					break;
				case '"':
					std::cout << "STRING(" << handle_string(_c, _buf.end()) << ")" << std::endl;
					break;
			}
		}
		_data += "line: " + _buf + "\n";
	}
	std::cout << _data << std::endl;

	return 0;
}
