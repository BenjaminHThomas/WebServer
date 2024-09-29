/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:28:22 by okoca             #+#    #+#             */
/*   Updated: 2024/09/29 14:15:23 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "json.hpp"
#include "lexer.hpp"
#include <fstream>
#include <temp.hpp>

class JSONParser
{
	private:
		typedef std::pair<std::string, JsonValue*>	Member;

		typedef JSONLexer::Tokens::iterator			iter;
		typedef JSONLexer::Tokens::const_iterator	const_iter;
	public:
		static JsonValue parse(std::ifstream &stream);

	private:
		static JsonValue	handle_tokens(const_iter &begin, const const_iter& end);
		static JsonValue	handle_object(const_iter &begin, const const_iter &end);
		static JsonValue	handle_array(const_iter &begin, const const_iter &end);

		static JsonValue::member_type handle_member(const_iter &begin, const const_iter &end);
};