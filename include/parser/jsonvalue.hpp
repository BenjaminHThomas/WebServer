/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jsonvalue.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:54:02 by okoca             #+#    #+#             */
/*   Updated: 2024/09/25 12:07:37 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "lexer.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>

class	JsonValue
{
	private:
		union
		{
			bool		_boolean;
			double		_decimal;
		};
	protected:
		JSONLexer::TokenType::Value _type;
	public:
		JsonValue();
		~JsonValue();
	protected:
	void delete_data(JsonValue *data);
};

class	JsonString : public JsonValue
{
	private:
		typedef std::string					value_type;
		typedef value_type::iterator		iter;
		typedef value_type::const_iterator	const_iter;
	private:
		value_type	_data;
	public:
		JsonString();
		~JsonString();
};

class	JsonObject : public JsonValue
{
	private:
		typedef std::map<std::string, JsonValue*>	value_type;
		typedef value_type::iterator				iter;
		typedef value_type::const_iterator			const_iter;
	private:
		value_type _data;
	public:
		JsonObject();
		~JsonObject();
};

class	JsonArray : public JsonValue
{
	private:
		typedef std::vector<JsonValue*> 			value_type;
		typedef value_type::iterator				iter;
		typedef value_type::const_iterator			const_iter;
	private:
		value_type _data;
	public:
		JsonArray();
		~JsonArray();
};