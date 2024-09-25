/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jsonvalue.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:43:34 by okoca             #+#    #+#             */
/*   Updated: 2024/09/25 12:13:05 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "jsonvalue.hpp"
#include <cstddef>

JsonValue::JsonValue()
{
}

JsonValue::~JsonValue()
{
}

void JsonValue::delete_data(JsonValue *data)
{
	if (data != NULL)
		delete data;
}

JsonArray::JsonArray()
{
}
JsonArray::~JsonArray()
{
	for (iter it = _data.begin(); it < _data.end(); it++)
		delete_data(*it);
}

JsonString::JsonString()
{}
JsonString::~JsonString()
{}

JsonObject::JsonObject()
{
}
JsonObject::~JsonObject()
{
	for (iter it = _data.begin(); it != _data.end(); it++)
		delete_data((*it).second);
}