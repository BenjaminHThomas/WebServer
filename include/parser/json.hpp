/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:54:02 by okoca             #+#    #+#             */
/*   Updated: 2024/09/28 21:51:56 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "lexer.hpp"
#include <cstddef>
#include <map>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <utility>
#include <vector>

class	JsonValue
{
	public:
		struct JsonType
		{
			enum Value
			{
				TARRAY,
				TOBJECT,
				TSTRING,
				TDECIMAL,
				TBOOLEAN,
				TNULL
			};
		};

		typedef std::string							string_type;
		typedef string_type::iterator				iter_str;
		typedef string_type::const_iterator			const_iter_str;

		typedef std::map<std::string, JsonValue>	object_type;
		typedef object_type::iterator				iter_obj;
		typedef object_type::const_iterator			const_iter_obj;

		typedef std::pair<std::string, JsonValue>	member_type;

		typedef std::vector<JsonValue> 			array_type;
		typedef array_type::iterator				iter_arr;
		typedef array_type::const_iterator			const_iter_arr;
	private:
		union
		{
			bool			_boolean;
			// double			_decimal;
			string_type*	_string;
			object_type*	_object;
			array_type*		_array;
		};
	protected:
		JsonType::Value _type;
	public:
		JsonValue();
		JsonValue(bool data);
		// JsonValue(double data);
		JsonValue(const string_type &str);
		JsonValue(const object_type &str);
		JsonValue(const array_type &str);
		JsonValue(JsonType::Value type);
		JsonValue(JSONLexer::Token token);
		JsonValue(const string_type& data, JsonType::Value type);

		JsonValue(const JsonValue &json);
		~JsonValue();

	public:
		JsonValue &operator=(const JsonValue &json);
	public:
		JsonValue &push_back(JsonValue value);
		JsonValue &insert(member_type value);
	public:
		JsonType::Value get_type() const;

		bool	to_bool(const std::string &str) const;
		double	to_double(const std::string &str) const;
		int64_t	to_number(const std::string &str) const;

		array_type &get_arr() { return (*_array); };
		object_type &get_obj() { return (*_object); };

	friend std::ostream & operator << (std::ostream &s, const JsonValue &json);
};

