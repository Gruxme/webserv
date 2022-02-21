/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/02/21 15:15:34 by abiari           ###   ########.fr       */
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
#define	SA	struct sockaddr

class sockets
{
	public:
		explicit sockets(unsigned short port);
		sockets(const sockets& x);
		virtual ~sockets();
		sockets&	operator=(const sockets& x);

		void					bindSock();
		void					listener(int maxLoad) const;
		int						acceptClient();
		std::vector<int>&		getClientsVec();
		int						getNumSds() const ;
		int						getMainSock() const ;
		unsigned short			getPort() const ;
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
		unsigned short		_port;
		struct sockaddr_in	_address;
};
