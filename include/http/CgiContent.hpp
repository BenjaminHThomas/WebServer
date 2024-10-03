/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiContent.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:36:22 by tsuchen           #+#    #+#             */
/*   Updated: 2024/10/03 14:51:06 by tsuchen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CGICONTENT_HPP
# define CGICONTENT_HPP

# include "AHttpData.hpp"

class CgiContent : public AHttpData
{
private:
	/* data */
public:
	CgiContent(std::string const &str);
	CgiContent(const CgiContent &other);
	virtual ~CgiContent();
	/* overload operators */
	CgiContent&	operator=(const CgiContent &other);

	void	parseHead();
};

#endif