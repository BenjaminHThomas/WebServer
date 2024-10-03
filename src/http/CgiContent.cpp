/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiContent.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:42:23 by tsuchen           #+#    #+#             */
/*   Updated: 2024/10/03 15:40:43 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiContent.hpp"

CgiContent::CgiContent(std::string const &str) : AHttpData(str, false) {
    
}

CgiContent::CgiContent(const CgiContent &other) : AHttpData(other.getRaw(), false) {
    *this = other;
}

CgiContent& CgiContent::operator=(const CgiContent &other) {
    if (this != &other) {
        AHttpData::operator=(other);
    }
    return *this;
}

CgiContent::~CgiContent() {}

void CgiContent::parseHead() {}