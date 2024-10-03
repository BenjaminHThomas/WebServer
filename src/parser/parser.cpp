/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:53:32 by okoca             #+#    #+#             */
/*   Updated: 2024/10/03 21:14:26 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json.hpp"
#include "lexer.hpp"
#include <iostream>
#include <parser.hpp>
#include <stdexcept>
#include <vector>

JsonValue::member_type JSONParser::handle_member(const_iter &begin, const const_iter &end)
{
	if (begin->type != JSONLexer::TokenType::STRING)
		throw std::runtime_error("invalid object: 'key' member is invalid");
	std::string str = begin->value;
	if (++begin == end)
		throw std::runtime_error("invalid object: [1] missing syntax in member");
	else if (begin->type != JSONLexer::TokenType::COLON)
		throw std::runtime_error("invalid object: [2] missing syntax in member");
	if (++begin == end)
		throw std::runtime_error("invalid object: [3] missing syntax in member");
	JsonValue val = handle_tokens(begin, end);
	return JsonValue::member_type(str, val);
}

JsonValue JSONParser::handle_object(const_iter &begin, const const_iter &end)
{
	JsonValue obj(JsonValue::JsonType::TOBJECT);
	for (begin++; begin < end; begin++)
	{
		if (begin->type == JSONLexer::TokenType::RCURLY)
			break ;
		obj.insert(handle_member(begin, end));
		if (++begin == end)
			throw std::runtime_error("invalid object: [1] missing syntax");
		else if (begin->type == JSONLexer::TokenType::RCURLY)
			break ;
		else if (begin->type != JSONLexer::TokenType::COMMA)
			throw std::runtime_error("invalid object: [2] missing syntax");
	}
	return obj;
}

JsonValue JSONParser::handle_array(const_iter &begin, const const_iter &end)
{
	JsonValue arr(JsonValue::JsonType::TARRAY);
	for (begin++; begin < end; begin++)
	{
		if (begin->type == JSONLexer::TokenType::RBRAC)
			break ;
		arr.push_back(handle_tokens(begin, end));
		if (++begin == end)
			throw std::runtime_error("invalid array: missing syntax");
		else if (begin->type == JSONLexer::TokenType::RBRAC)
			break ;
		else if (begin->type != JSONLexer::TokenType::COMMA)
			throw std::runtime_error("invalid array: missing syntax");
	}
	return arr;
}

JsonValue	JSONParser::handle_tokens(const_iter &begin, const const_iter& end)
{
	try
	{
		switch (begin->type)
		{
			case JSONLexer::TokenType::LCURLY:
				return handle_object(begin, end);
			case JSONLexer::TokenType::LBRAC:
				return handle_array(begin, end);
			case JSONLexer::TokenType::STRING:
			case JSONLexer::TokenType::DECIMAL:
			case JSONLexer::TokenType::VTRUE:
			case JSONLexer::TokenType::VFALSE:
				return JsonValue(*begin);
			case JSONLexer::TokenType::VNULL:
				return JsonValue();
			default:
				break;
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "error: " << e.what() << std::endl;
	}
	throw std::runtime_error("bad token");
}

JsonValue JSONParser::parse(std::ifstream &stream)
{
	JSONLexer::Tokens tokens = JSONLexer::lex(stream);
	JSONLexer::Tokens::const_iterator it = tokens.begin();
	JsonValue val;
	try
	{
		val = handle_tokens(it, tokens.end());
	}
	catch (const std::exception &e)
	{
		std::cerr << "error: " << e.what() << std::endl;
	}
	return val;
}
