/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:53:32 by okoca             #+#    #+#             */
/*   Updated: 2024/09/26 22:46:37 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json.hpp"
#include "lexer.hpp"
#include <parser.hpp>
#include <stdexcept>
#include <utility>
#include <vector>


JsonValue::member_type JSONParser::handle_member(iter &begin, const const_iter &end)
{
	if (begin->type != JSONLexer::TokenType::STRING)
		throw std::runtime_error("invalid object: 'key' member is invalid");
	std::string str = begin->value;
	if (++begin == end || begin->type != JSONLexer::TokenType::COLON || ++begin == end)
		throw std::runtime_error("invalid object: missing syntax in member");
	JsonValue val = handle_tokens(begin, end);
	return JsonValue::member_type(str, val);
}

JsonValue JSONParser::handle_object(iter &begin, const const_iter &end)
{
	JsonValue obj(JsonValue::JsonType::TOBJECT);
	for (; begin < end; begin++)
	{
		if (begin->type == JSONLexer::TokenType::RCURLY)
			break ;
		// obj.insert(handle_member(begin, end));
		// obj[begin->value] =
		try
		{
			// obj.get_obj()[begin->value] = JsonValue(*(++(++begin)));
			JsonValue::member_type member(begin->value, JsonValue(*(++(++begin))));
			// obj.get_obj().insert(member);
			// obj.insert(std::make_pair(begin->value, handle_tokens(++(++begin), end)));
		}
		catch (const std::exception &e)
		{
			std::cerr << "HHerrorHH: " << e.what() << std::endl;
		}
		// obj.insert(std::make_pair(begin->value, handle_tokens(++(++begin), end)));
		if ((++begin) == end || (begin->type != JSONLexer::TokenType::RCURLY && begin->type != JSONLexer::TokenType::COMMA))
			throw std::runtime_error("invalid object: missing syntax");
	}
	return obj;
}

JsonValue JSONParser::handle_array(iter &begin, const const_iter &end)
{
	JsonValue arr(JsonValue::JsonType::TARRAY);
	for (; begin < end; begin++)
	{
		if (begin->type == JSONLexer::TokenType::RBRAC)
			break ;
		arr.push_back(handle_tokens(begin, end));
		if ((++begin) == end || (begin->type != JSONLexer::TokenType::RBRAC && begin->type != JSONLexer::TokenType::COMMA))
			throw std::runtime_error("invalid array: missing syntax");
	}
	return arr;
}

JsonValue	JSONParser::handle_tokens(iter &begin, const const_iter& end)
{
	switch (begin->type)
	{
		case JSONLexer::TokenType::LCURLY:
			return handle_object(++begin, end);
		case JSONLexer::TokenType::LBRAC:
			return handle_array(++begin, end);
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
	throw std::runtime_error("bad token");
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