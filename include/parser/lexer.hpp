/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:53:02 by okoca             #+#    #+#             */
/*   Updated: 2024/09/25 11:40:06 by okoca            ###   ########.fr       */
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
		Tokens _tokens;
	public:
		JSONLexer();
		JSONLexer(std::ifstream &stream);
		~JSONLexer();
	private:
		void	loop(iter begin, iter end);

	private:
		Token handle_string(iter &begin, const iter& end);
		Token handle_complete(iter &begin, const iter &end);

	public:
		const std::vector<Token> &get_tokens() const;
	public:
		void		debug() const;
		std::string	token_type_to_string(JSONLexer::TokenType::Value type) const;
};