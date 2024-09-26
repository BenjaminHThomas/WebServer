/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:53:32 by okoca             #+#    #+#             */
/*   Updated: 2024/09/26 11:32:29 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json.hpp"
#include "lexer.hpp"
#include <parser.hpp>
#include <stdexcept>
#include <utility>
#include <vector>


JSONParser::Member JSONParser::handle_member(iter &begin, const const_iter &end)
{
	if (begin->type != JSONLexer::TokenType::STRING)
	{
		std::cerr << begin->type << std::endl;
		std::cerr << (++begin)->type << std::endl;
		throw std::runtime_error("invalid object: 'key' member is invalid");
	}
	const std::string &str = begin->value;
	if (++begin == end || begin->type != JSONLexer::TokenType::COLON || ++begin == end)
		throw std::runtime_error("invalid object: missing syntax in member");
	JsonValue *val = handle_tokens(begin, end);
	return std::make_pair(str, val);
}

JsonObject *JSONParser::handle_object(iter &begin, const const_iter &end)
{
	JsonObject *obj = new JsonObject;

	for (; begin < end; begin++)
	{
		if (begin->type == JSONLexer::TokenType::RCURLY)
			break ;
		obj->insert(handle_member(begin, end));
		if ((++begin) == end)
			throw std::runtime_error("invalid object: missing syntax");
		if (begin->type != JSONLexer::TokenType::RCURLY && begin->type != JSONLexer::TokenType::COMMA)
			throw std::runtime_error("invalid object: missing syntax");
	}
	return obj;
}

JsonValue	JSONParser::handle_tokens(iter &begin, const const_iter& end)
{
	for (; begin < end; begin++)
	{
		switch (begin->type) {
			case JSONLexer::TokenType::LCURLY:
				return handle_object(++begin, end);
			case JSONLexer::TokenType::LBRAC:
			case JSONLexer::TokenType::STRING:
			case JSONLexer::TokenType::DECIMAL:
			case JSONLexer::TokenType::VTRUE:
			case JSONLexer::TokenType::VFALSE:
				return JsonValue(begin->value);
			case JSONLexer::TokenType::VNULL:
				return JsonValue();
			default:
				break;
		}
	}
	throw std::runtime_error("bad token");
	// return new JsonValue;
}

JSONParser::JSONParser(std::ifstream &stream)
{
	JSONLexer lex(stream);

	JSONLexer::Tokens tokens = lex.get_tokens();
	JSONLexer::Tokens::iterator it = tokens.begin();
	try
	{
		JsonValue val = handle_tokens(it, tokens.end());
		std::cout << val.get_type() << std::endl;;
	}
	catch (const std::exception &e)
	{
		std::cerr << "error: " << e.what() << std::endl;
	}
}

JSONParser::~JSONParser()
{}