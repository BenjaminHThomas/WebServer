/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jsonvalue.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:43:34 by okoca             #+#    #+#             */
/*   Updated: 2024/09/25 16:39:52 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "jsonvalue.hpp"
#include "lexer.hpp"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

JsonValue::JsonValue() : _type(JsonType::JNULL)
{
}

JsonValue::JsonValue(JsonType::Value type) : _type(type)
{
}

JsonValue::JsonValue(bool data) : _type(JsonType::JBOOLEAN)
{
	this->_boolean = data;
}

JsonValue::JsonValue(double data) : _type(JsonType::JDECIMAL)
{
	this->_decimal = data;
}

JsonValue::~JsonValue()
{
}

void JsonValue::delete_data(JsonValue *data)
{
	if (data != NULL)
		delete data;
}

JsonArray::JsonArray() : JsonValue(JsonValue::JsonType::JARRAY)
{
}

JsonArray::JsonArray(value_type data) : JsonValue(JsonValue::JsonType::JARRAY)
{
	_data = data;
}

JsonArray &JsonArray::insert(JsonValue *val)
{
	_data.push_back(val);
	return *this;
}

JsonArray::~JsonArray()
{
	for (iter it = _data.begin(); it < _data.end(); it++)
		delete_data(*it);
}

JsonString::JsonString(): JsonValue(JsonValue::JsonType::JSTRING)
{
}

JsonString::JsonString(std::string data): JsonValue(JsonValue::JsonType::JSTRING)
{
	_data = data;
}

JsonString::~JsonString()
{}

JsonObject::JsonObject() : JsonValue(JsonValue::JsonType::JOBJECT)
{
}

JsonObject::JsonObject(value_type data) : JsonValue(JsonValue::JsonType::JOBJECT)
{
	this->_data = data;
}

JsonObject &JsonObject::insert(std::string key, JsonValue *value)
{
	if (_data.find(key) != _data.end())
		throw std::runtime_error("duplicate keys in object.");
	this->_data[key] = value;

	return *this;
}

JsonObject &JsonObject::insert(std::pair<std::string, JsonValue *> value)
{
	if (_data.find(value.first) != _data.end())
		throw std::runtime_error("duplicate keys in object.");
	this->_data.insert(value);

	return *this;
}

JsonObject::~JsonObject()
{
	for (iter it = _data.begin(); it != _data.end(); it++)
		delete_data((*it).second);
}