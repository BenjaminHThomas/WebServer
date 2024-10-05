/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 09:35:56 by okoca             #+#    #+#             */
/*   Updated: 2024/10/05 18:37:45 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>

bool is_directory(const std::string &path);

bool end_with_json(const std::string &c);

enum CgiResult {
	SUCCESS,
	TIMEDOUT,
	NOTFOUND,
	ERROR
};