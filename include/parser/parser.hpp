/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:28:22 by okoca             #+#    #+#             */
/*   Updated: 2024/09/26 22:13:46 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "json.hpp"
#include "lexer.hpp"
#include <temp.hpp>

class JSONParser
{
	private:
		typedef std::pair<std::string, JsonValue*>	Member;

		typedef JSONLexer::Tokens::iterator			iter;
		typedef JSONLexer::Tokens::const_iterator	const_iter;
	public:
		JSONParser(std::ifstream &stream);
		~JSONParser();

	private:
		JsonValue	handle_tokens(iter &begin, const const_iter& end);
		JsonValue	handle_object(iter &begin, const const_iter &end);
		JsonValue	handle_array(iter &begin, const const_iter &end);

		JsonValue::member_type handle_member(iter &begin, const const_iter &end);
};