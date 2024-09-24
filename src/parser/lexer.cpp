/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:07:13 by okoca             #+#    #+#             */
/*   Updated: 2024/09/24 17:10:59 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <temp.hpp>
#include <lexer.hpp>
#include <fstream>

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

JSONLexer::JSONLexer()
{}

void	JSONLexer::loop(iter begin, iter end)
{
	for (; begin < end; begin++)
	{
		switch (*begin)
		{
			case '{':
				_tokens.push_back(Token(JSONLexer::TokenType::LCURLY));
				break;
			case '}':
				_tokens.push_back(Token(JSONLexer::TokenType::RCURLY));
				break;
			case '[':
				_tokens.push_back(Token(JSONLexer::TokenType::LBRAC));
				break;
			case ']':
				_tokens.push_back(Token(JSONLexer::TokenType::RBRAC));
				break;
			case ',':
				_tokens.push_back(Token(JSONLexer::TokenType::COMMA));
				break;
			case ':':
				_tokens.push_back(Token(JSONLexer::TokenType::COLON));
				break;
			case '"':
				_tokens.push_back(handle_string(begin, end));
				break;
			case '\t':
			case '\r':
			case '\n':
			case ' ':
				break;
			default:
					_tokens.push_back(handle_complete(begin, end));
				break;
		}
	}
}

JSONLexer::JSONLexer(std::ifstream &stream)
{
	std::string	_data;
	std::string	_buf;
	while (std::getline(stream, _buf))
	{
		loop(_buf.begin(), _buf.end());
		_data += "line: " + _buf + "\n";
	}
	std::cout << _data << std::endl;
}

JSONLexer::~JSONLexer()
{

}

JSONLexer::Token JSONLexer::handle_string(std::string::iterator &begin, const std::string::iterator &end)
{
	std::string s;
	if (std::find(begin++, end, '"') == end)
		throw std::runtime_error("error syntax: no closing quote [handle_string()]");
	while (begin < end)
	{
		if (*begin == '"')
			break;
		s.push_back(*begin);
		begin++;
	}
	return Token(JSONLexer::TokenType::STRING, s);
}

JSONLexer::Token JSONLexer::handle_complete(iter &begin, const iter &end)
{
	Token token;

	std::string::iterator end_word = std::find_first_of(begin, end, all_chars.begin(), all_chars.end());
	if (end_word == end)
		throw std::runtime_error("error syntax: syntax [handle_complete()]");

	std::string word;
	for (; begin < end_word; begin++)
	{
		word.push_back(*begin);
	}

	if (word == "true")
		token.type = JSONLexer::TokenType::VTRUE;
	else if (word == "false")
		token.type = JSONLexer::TokenType::VFALSE;
	else if (word == "null")
		token.type = JSONLexer::TokenType::VNULL;
	else if (!word.empty() && word.find_first_not_of("0123456789.") == std::string::npos)
	{
		token.type = JSONLexer::TokenType::DECIMAL;
		token.value = word;
	}
	else
		throw std::runtime_error("error syntax: invalid 'word' token [handle_complete()]");
	return token;
}