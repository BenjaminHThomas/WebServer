/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIManager.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 12:06:58 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/01 13:20:58 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CGIMANAGER_HPP
# define CGIMANAGER_HPP

#include <iostream>
#include "EventHandler.hpp"

struct CGIInfo {
			int clientFd;
			int pipeFd;
			pid_t pid;
			std::string output;
			bool isFinished;
};

class CGIManager
{
	public:
		std::map<int, CGIInfo*> _cgiProcesses;

	public:
		CGIManager ();
		~CGIManager ();

	public:
		void addCgiProcess(int clientFd, int pipeFd, pid_t pid);
		bool isInManager(int pipeFd);
		void readCGIOutput(int pipeFd);
		void deleteFromCGI(int pipeFd);
		bool isResponseComplete(int pipefd);
};

#endif /* CGIMANAGER_HPP */
