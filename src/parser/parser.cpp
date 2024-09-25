/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:53:32 by okoca             #+#    #+#             */
/*   Updated: 2024/09/25 09:52:54 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.hpp"
#include <parser.hpp>
#include <vector>

void	JSONParser::handle_tokens(JSONLexer::Tokens tokens)
{
	(void)tokens;
}

JSONParser::JSONParser(std::ifstream &stream)
{
	JSONLexer lex(stream);

	// JSONLexer::Tokens tokens = lex.get_tokens();
	handle_tokens(lex.get_tokens());
}

JSONParser::~JSONParser()
{}