/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIManager.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 12:06:58 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/01 16:54:22 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CGIMANAGER_HPP
# define CGIMANAGER_HPP

#define BUFFER_SIZE 30720

#include <iostream>
#include <map>
#include <unistd.h>
#include <cstdlib>

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
