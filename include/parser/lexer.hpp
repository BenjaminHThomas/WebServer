/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:53:02 by okoca             #+#    #+#             */
/*   Updated: 2024/09/29 14:11:27 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include <string>

class JSONLexer
{
	public:
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

		typedef std::string::iterator iter;
		typedef std::string::const_iterator const_iter;
		struct Token
		{
			TokenType::Value	type;
			std::string			value;

			Token() {}
			Token(TokenType::Value _val) : type(_val) {}
			Token(TokenType::Value _val, std::string _str) : type(_val), value(_str) {}
		};

		typedef std::vector<Token> Tokens;
	private:
		static Tokens _tokens;
	public:
		static Tokens lex(std::ifstream &stream);
	private:
		static void	loop(iter begin, iter end);

	private:
		static Token handle_string(iter &begin, const iter& end);
		static Token handle_complete(iter &begin, const iter &end);
		static std::string	token_type_to_string(JSONLexer::TokenType::Value type);

	public:
		static void		debug();
};