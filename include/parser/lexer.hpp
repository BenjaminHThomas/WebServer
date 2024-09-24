/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:53:02 by okoca             #+#    #+#             */
/*   Updated: 2024/09/24 16:57:14 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include <string>

class JSONLexer
{
	private:
	  	struct TokenType
		{
			enum Value
			{
				LCURLY,
				RCURLY,
				LBRAC,
				RBRAC,
				COMMA,
				COLON,
				VTRUE,
				VFALSE,
				VNULL,
				STRING,
				DECIMAL,
			};
		};
		struct Token
		{
			TokenType::Value	type;
			std::string			value;

			Token() {}
			Token(TokenType::Value _val) : type(_val) {}
			Token(TokenType::Value _val, std::string _str) : type(_val), value(_str) {}
		};

		typedef std::string::iterator iter;
		typedef std::string::const_iterator const_iter;
	private:
		std::vector<Token> _tokens;
	public:
		JSONLexer();
		JSONLexer(std::ifstream &stream);
		~JSONLexer();
	private:
		void	loop(iter begin, iter end);
	private:
		Token handle_string(iter &begin, const iter& end);
		Token handle_complete(iter &begin, const iter &end);
		// std::string handle_complete(iter &begin, const iter& end);
};