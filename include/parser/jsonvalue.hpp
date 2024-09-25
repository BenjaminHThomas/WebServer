/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jsonvalue.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:54:02 by okoca             #+#    #+#             */
/*   Updated: 2024/09/25 16:18:32 by okoca            ###   ########.fr       */
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
	public:
		struct JsonType
		{
			enum Value
			{
				JARRAY,
				JOBJECT,
				JSTRING,
				JDECIMAL,
				JBOOLEAN,
				JNULL
			};
		};
	private:
		union
		{
			bool		_boolean;
			double		_decimal;
		};
	protected:
		JsonType::Value _type;
	public:
		JsonValue(bool data);
		JsonValue(double data);
		JsonValue(JsonType::Value type);
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
		JsonString(std::string data);
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
		JsonObject(value_type data);
		JsonObject();
		~JsonObject();
	public:
		JsonObject& insert(std::string key, JsonValue *value);
		JsonObject &insert(std::pair<std::string, JsonValue *>);
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
		JsonArray(value_type data);
		JsonArray();
		~JsonArray();
	public:
		JsonArray &insert(JsonValue *val);
};