/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/02/23 11:11:41 by aabounak         ###   ########.fr       */
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
		explicit sockets(ServerConfigClass conf);
		sockets(const sockets& x);
		virtual ~sockets();
		sockets&	operator=(const sockets& x);

		void					bindSock();
		void					listener(int maxLoad) const;
		int						acceptClient();
		std::vector<int>&		getClientsVec();
		int						getNumSds() const ;
		int						getMainSock() const ;
		ServerConfigClass			getConfig() const ;
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
		int					_mainSd, _nsds;
		std::vector<int>	_clients;
		ServerConfigClass		_config;
		struct sockaddr_in	_address;
};
