/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:43:34 by okoca             #+#    #+#             */
/*   Updated: 2024/09/26 11:03:11 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json.hpp"
#include "lexer.hpp"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

JsonValue &JsonValue::operator=(const JsonValue &json)
{
	if (this != &json)
	{
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
		_type = json._type;
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
	_string = new std::string(value);
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
	_array->push_back(value);
	return *this;
}

JsonValue &JsonValue::insert(member_type value)
{
	if (_object->find(value.first) != _object->end())
		throw std::runtime_error("duplicate keys in object.");
	this->_object->insert(value);

	return *this;
}
