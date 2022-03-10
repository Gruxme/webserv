/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/03/09 15:42:34 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <vector>
#include "../config/ServerConfigClass.hpp"
#define	SA	struct sockaddr

class sockets
{
	public:
		sockets();
		explicit sockets(ServerConfigClass conf, int maxLoad);
		sockets(const sockets& x);
		~sockets();
		sockets&	operator=(const sockets& x);
		
		int						acceptClient();
		std::vector<int>&		getClientsVec();
		int						getNumSds() const ;
		int						getMainSock() const ;
		ServerConfigClass		getConfig() const ;
		struct sockaddr_in		getAddr() const;

		class socketErr: public std::exception{
			public:
				explicit socketErr(const std::string& errStr) throw() : _errStr(errStr) {}
				~socketErr() throw(){}
				virtual const char *what() const throw(){
					perror(_errStr.c_str());
					return "";
				}
			private:
				std::string	_errStr;
		};
	private:
		void					_listener(int maxLoad);
		void 					_bindSock();
		
	private:
		int						_mainSd, _nsds;
		std::vector<int>		_clients;
		ServerConfigClass		_config;
		struct sockaddr_in		_address;
};
