/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:43:34 by okoca             #+#    #+#             */
/*   Updated: 2024/09/30 10:34:45 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json.hpp"
#include "lexer.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>

JsonValue &JsonValue::operator=(const JsonValue &json)
{
	if (this != &json)
	{
		_type = json._type;
		switch (_type)
		{
			case (JsonType::TBOOLEAN): _boolean = json._boolean; break;
			case (JsonType::TDECIMAL): // _decimal = json._decimal; break;
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
			_type = JsonType::TDECIMAL;
			// if (token.value.find('.') == std::string::npos)
			// 	_decimal = std::atoll(token.value.c_str());
			// else
			// _decimal = std::atof(token.value.c_str());
			_string = new std::string(token.value);
			break;
		default:
			break;
	}
}

JsonValue::JsonValue(bool data) : _type(JsonType::TBOOLEAN)
{
	_boolean = data;
}

JsonValue::JsonValue(const string_type& data, JsonType::Value type) : _type(type)
{
	_string = new std::string(data);
}

JsonValue::JsonValue(const string_type &value) : _type(JsonType::TSTRING)
{
	_string = new std::string(value);
}


JsonValue::JsonValue(const char *str) : _type(JsonType::TSTRING)
{
	_string = new std::string(str);
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
		case (JsonType::TOBJECT):	delete _object;	break;
		case (JsonType::TDECIMAL):
		case (JsonType::TSTRING):	delete _string;	break;
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

double JsonValue::to_double(const std::string &str) const
{
	return std::atof(str.c_str());
}

int64_t JsonValue::to_number(const std::string &str) const
{
	return std::atoll(str.c_str());
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
			s << *json._string;
			break ;
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
			level++;
			for (JsonValue::const_iter_arr	it = json._array->begin(); it < json._array->end(); it++)
			{
				for (int i = 0; i < level; i++)
					s << "    ";
				s << *it << ",\n";
			}
			level--;
			for (int i = 0; i < level; i++)
				s << "    ";
			s << "]";
			break;
	}
	return s;
}

const JsonValue &JsonValue::validate(JsonValue::JsonType::Value t) const
{
	if (t != _type)
		throw std::runtime_error("invalid type, expected");
	return *this;
}

JsonValue &JsonValue::operator[](int idx) const
{
	validate(JsonType::TARRAY);
	JsonValue &val = _array->at(idx);
	return val;
}

JsonValue &JsonValue::operator[](const char *str) const
{
	validate(JsonType::TOBJECT);
	JsonValue &val = _object->at(str);
	return val;
}

JsonValue &JsonValue::operator[](const std::string &s) const
{
	validate(JsonType::TOBJECT);
	JsonValue &val = _object->at(s);
	return val;
}

const std::string &JsonValue::as_string() const
{
	try
	{
		validate(JsonType::TSTRING);
	}
	catch (const std::exception &e)
	{
		validate(JsonType::TDECIMAL);
	}
	return *_string;
}

int64_t	JsonValue::as_number() const
{
	validate(JsonType::TDECIMAL);
	return to_number(*_string);
}
double	JsonValue::as_decimal() const
{
	validate(JsonType::TDECIMAL);
	return to_double(*_string);
}


JsonValue::const_iter_arr	JsonValue::begin_arr() const
{
	validate(JsonType::TARRAY);
	return _array->begin();
}

JsonValue::const_iter_arr	JsonValue::end_arr() const
{
	validate(JsonType::TARRAY);
	return _array->end();
}

JsonValue::const_iter_obj	JsonValue::begin_obj() const
{
	validate(JsonType::TOBJECT);
	return _object->begin();
}

JsonValue::const_iter_obj	JsonValue::end_obj() const
{
	validate(JsonType::TOBJECT);
	return _object->end();
}
