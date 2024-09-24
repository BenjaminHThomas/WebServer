/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:53:32 by okoca             #+#    #+#             */
/*   Updated: 2024/09/24 16:18:28 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.hpp"
#include <parser.hpp>
#include <stdexcept>



JSONParser::JSONParser(std::ifstream &stream)
{
	JSONLexer lex(stream);
}

JSONParser::~JSONParser()
{

}