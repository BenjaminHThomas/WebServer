/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:43:34 by okoca             #+#    #+#             */
/*   Updated: 2024/09/28 21:17:05 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json.hpp"
#include "lexer.hpp"
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

JsonValue &JsonValue::operator=(const JsonValue &json)
{
	if (this != &json)
	{
		_type = json._type;
		switch (_type)
		{
			case (JsonType::TBOOLEAN): _boolean = json._boolean; break;
			case (JsonType::TDECIMAL): _decimal = json._decimal; break;
			case (JsonType::TSTRING):
				_string = new string_type(*json._string);
				break;
			case (JsonType::TARRAY):
				_array = new array_type(*json._array);
				break;
			case (JsonType::TOBJECT):
				_object = new object_type(*json._object);
				break;
			default:
				break;
		}
	}
	return *this;
}

JsonValue::JsonValue(const JsonValue &json)
{
	*this = json;
}

JsonValue::JsonValue() : _type(JsonType::TNULL)
{
}

JsonValue::JsonValue(JsonType::Value type) : _type(type)
{
	switch (_type)
	{
		case (JsonType::TSTRING):
			_string = new string_type();
			break;
		case (JsonType::TARRAY):
			_array = new array_type();
			break;
		case (JsonType::TOBJECT):
			_object = new object_type();
			break;
		default:
			break;
	}
}

JsonValue::JsonValue(JSONLexer::Token token)
{
	switch (token.type)
	{
		case (JSONLexer::TokenType::STRING):
			_type = JsonType::TSTRING;
			_string = new std::string(token.value);
			break;
		case (JSONLexer::TokenType::VFALSE):
			_type = JsonType::TBOOLEAN;
			_boolean = false;
			break;
		case (JSONLexer::TokenType::VTRUE):
			_type = JsonType::TBOOLEAN;
			_boolean = true;
			break;
		case (JSONLexer::TokenType::DECIMAL):
			_type = JsonType::TBOOLEAN;
			_decimal = to_number(token.value);
			break;
		default:
			break;
	}
}

JsonValue::JsonValue(bool data) : _type(JsonType::TBOOLEAN)
{
	_boolean = data;
}

JsonValue::JsonValue(double data) : _type(JsonType::TDECIMAL)
{
	_decimal = data;
}

JsonValue::JsonValue(const string_type &value) : _type(JsonType::TSTRING)
{
	try
	{
	_string = new std::string(value);
	}
	catch (const std::exception &e)
	{
		std::cerr << "no way its here: " << e.what() << std::endl;
	}
}

JsonValue::JsonValue(const object_type &value) : _type(JsonType::TOBJECT)
{
	_object = new object_type(value);
}

JsonValue::JsonValue(const array_type &value) : _type(JsonType::TARRAY)
{
	_array = new array_type(value);
}

JsonValue::~JsonValue()
{
	switch (_type)
	{
		case (JsonType::TARRAY):	delete _array;	break;
		case (JsonType::TSTRING):	delete _string;	break;
		case (JsonType::TOBJECT):	delete _object;	break;
		default: break;
	}
}

JsonValue::JsonType::Value JsonValue::get_type() const
{
	return _type;
}

JsonValue &JsonValue::push_back(JsonValue value)
{
	if (_type != JsonType::TARRAY)
		throw std::runtime_error("invalid json type: expected array");
	_array->push_back(value);
	return *this;
}

JsonValue &JsonValue::insert(member_type value)
{
	if (_type != JsonType::TOBJECT)
		throw std::runtime_error("invalid json type: expected object");
	if (_object->find(value.first) != _object->end())
		throw std::runtime_error("duplicate keys in object.");
	this->_object->insert(value);
	return *this;
}

bool JsonValue::to_bool(const std::string &str) const
{
	if (str != "true" && str != "false")
		throw std::runtime_error("invalid conversion: trying to csat non-boolean string as boolean");
	return str == "true" ? true : false;
}

double JsonValue::to_number(const std::string &str) const
{
	return std::atof(str.c_str());
}

std::ostream &operator<<(std::ostream &s, const JsonValue &json)
{
	static int	level = 0;
	switch (json._type)
	{
		case (JsonValue::JsonType::TBOOLEAN):
			s << (json._boolean ? "true" : "false");
			break;
		case (JsonValue::JsonType::TDECIMAL):
			s << json._decimal;
			break;
		case (JsonValue::JsonType::TSTRING):
			s << '"' << *json._string << '"';
			break;
		case (JsonValue::JsonType::TNULL):
			s << "null";
			break;
		case (JsonValue::JsonType::TOBJECT):
			s << "{\n";
			level++;
			for (JsonValue::const_iter_obj	it = json._object->begin(); it != json._object->end(); it++)
			{
				for (int i = 0; i < level; i++)
					s << "    ";
				s << it->first << ": " << it->second << ",\n";
			}
			level--;
			for (int i = 0; i < level; i++)
				s << "    ";
			s << "}";
			break;
		case (JsonValue::JsonType::TARRAY):
			s << "[\n";
			for (JsonValue::const_iter_arr	it = json._array->begin(); it < json._array->end(); it++)
			{
				s << "    " << *it << ",";
			}
			s << "\n]";
			break;
	}
	return s;
}