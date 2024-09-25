/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:53:32 by okoca             #+#    #+#             */
/*   Updated: 2024/09/25 16:40:52 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "jsonvalue.hpp"
#include "lexer.hpp"
#include <parser.hpp>
#include <stdexcept>
#include <utility>
#include <vector>


JSONParser::Member JSONParser::handle_member(iter &begin, const const_iter &end)
{
	if (begin->type != JSONLexer::TokenType::STRING)
		throw std::runtime_error("invalid object: 'key' member is invalid");
	const std::string &str = begin->value;
	if (++begin == end || ++begin == end)
		throw std::runtime_error("invalid object: missing syntax in member");
	JsonValue *val = handle_tokens(begin, end);
	return std::make_pair(str, val);
}

JsonObject *JSONParser::handle_object(iter &begin, const const_iter &end)
{
	JsonObject *obj = new JsonObject;

	for (; begin < end; begin++)
	{
		if ((*begin).type == JSONLexer::TokenType::RCURLY)
			break ;
		obj->insert(handle_member(begin, end));
		//handle comma, check what handle_member/handle_tokens might give you back
	}
	return obj;
}

JsonValue*	JSONParser::handle_tokens(iter &begin, const const_iter& end)
{
	for (; begin < end; begin++)
	{
		switch ((*begin).type) {
			case JSONLexer::TokenType::LCURLY:
				return handle_object(begin, end);
			case JSONLexer::TokenType::LBRAC:
			case JSONLexer::TokenType::STRING:
			case JSONLexer::TokenType::DECIMAL:
			case JSONLexer::TokenType::VTRUE:
			case JSONLexer::TokenType::VFALSE:
			default:
				break;
		}
	}
	return new JsonValue;
}

JSONParser::JSONParser(std::ifstream &stream)
{
	JSONLexer lex(stream);

	JSONLexer::Tokens tokens = lex.get_tokens();
	JSONLexer::Tokens::iterator it = tokens.begin();
	handle_tokens(it, tokens.end());
}

JSONParser::~JSONParser()
{}