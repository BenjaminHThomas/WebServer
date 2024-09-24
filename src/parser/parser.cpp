/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:53:32 by okoca             #+#    #+#             */
/*   Updated: 2024/09/24 10:57:16 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.hpp>

JSONParser::JSONParser(std::ifstream &stream)
{
	std::string	_data;
	std::string	_buf;
	while (std::getline(stream, _buf))
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
				case '[':
					std::cout << "LBRAC" << std::endl;
					break;
				case ']':
					std::cout << "RBRAC" << std::endl;
					break;
				case ',':
					std::cout << "COMMA" << std::endl;
					break;
				case ':':
					std::cout << "COLON" << std::endl;
					break;
				case '"':
					std::cout << "STRING(" << handle_string(_c, _buf.end()) << ")" << std::endl;
					break;
				case '\t':
				case '\r':
				case '\n':
				case ' ':
					break;
				default:
					if (!handle_complete(_c, _buf.end()))
						return 1;
					break;
			}
		}
		_data += "line: " + _buf + "\n";
	}
	std::cout << _data << std::endl;
}

JSONParser::~JSONParser()
{

}