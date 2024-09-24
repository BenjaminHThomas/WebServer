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

#include <temp.hpp>

static const std::string whitespace = "\t\r\n ";

static const std::string all_chars = "{}[];,\"" + whitespace;

struct Keywords
{
	enum Value
	{
		KTRUE,
		KFALSE,
		KNULL
	};
};

std::map<Keywords::Value, std::string> init_keywords()
{
	std::map<Keywords::Value, std::string> map;

	map.insert(std::make_pair(Keywords::KTRUE, "true"));
	map.insert(std::make_pair(Keywords::KFALSE, "false"));
	map.insert(std::make_pair(Keywords::KNULL, "null"));
	return map;
}

static const std::map<Keywords::Value, std::string> keywords = init_keywords();

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

bool handle_complete(std::string::iterator &begin, const std::string::iterator &end)
{

	std::string::iterator end_word = std::find_first_of(begin, end, all_chars.begin(), all_chars.end());
	if (end_word == end)
	{
		std::cerr << "error syntax: handle_complete()" << std::endl;
		return false;
	}

	std::string word;
	for (; begin < end_word; begin++)
	{
		word.push_back(*begin);
	}

	if (word == "true" || word == "false" || word == "null")
		std::cout << "KEYWORD(" << word << ")" << std::endl;
	else if (word.find_first_not_of("0123456789.") == std::string::npos)
	{
		std::cout << "NUMBER(" << word << ")" << std::endl;
	}
	else
	{
		std::cerr << "error syntax: invalid 'word' handle_complete()" << std::endl;
		return false;
	}
	return true;
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

	return 0;
}
