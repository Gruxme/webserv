/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2021/12/23 18:05:37 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sys/time.h>
#define	SA	struct sockaddr

class socketsIO
{
	public:
		socketsIO(unsigned short port): _sd(-1), _clients(), _port(port){
			int	opt = 1;
			if((_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
				throw socketErr("socket: ");
			if(setsockopt(_sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
				throw socketErr("setsockopt: ");
		}
		
		socketsIO(const socketsIO& x){ 
			this->operator=(x);
		}
		
		virtual ~socketsIO(){
			close(_sd);
		}
		socketsIO&	operator=(const socketsIO& x){
			_sd = x._sd;
			_clients = x._clients;
			_port = x._port;
			_address = x._address;
			return *this;
		}
		void	bindSock(){
			_address.sin_addr.s_addr = INADDR_ANY;
			_address.sin_port = htons(_port);
			_address.sin_family = AF_INET;
			if(bind(_sd, (SA *)&_address, sizeof(_address)) < 0)
				throw socketErr("bind: ");
		}
		void	listener(int maxLoad){
			if(listen(_sd, maxLoad) < 0)
				throw socketErr("Listen: ");
			std::cout << "Socket listening on port " << _port << std::endl;
		}
		void	acceptClient(){
			int	newClient = -1, addrlen = sizeof(_address);			
			if((newClient = accept(_sd, (SA *)&_address, (socklen_t *)&addrlen)) < 0)
				throw socketErr("accept: ");
			_clients.push_back(newClient);
		}
		void	setNonBlock(){
			fcntl(_sd, F_SETFL, O_NONBLOCK);
		}
		std::vector<int>	getClientsVec() {return _clients; }
		int					getMainSock() {return _sd; }
		class socketErr: public std::exception{
			public:
				socketErr(std::string errStr) throw() : _errStr(errStr) {}
				~socketErr() throw(){}
				virtual const char *what() const throw(){
					perror(_errStr.c_str());
					return "";
				}
			private:
				std::string	_errStr;
		};
	private:
		int					_sd;
		std::vector<int>	_clients;
		unsigned short		_port;
		struct	sockaddr_in	_address;
};
