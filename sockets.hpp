/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/01/13 19:06:53 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#define	SA	struct sockaddr

class sockets
{
	public:
		explicit sockets(unsigned short port): _mainSd(-1), _clients(), _port(port), _address(){
			int	opt = 1;
			if((_mainSd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
				throw socketErr("socket: ");
			if(setsockopt(_mainSd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
				throw socketErr("setsockopt: ");
			_nsds = 1;
		}
		
		sockets(const sockets& x) : _mainSd(x._mainSd), _nsds(x._nsds), _port(x._port), _address(x._address){}
		
		virtual ~sockets(){
			std::vector<int>::iterator it = _clients.begin(), ite = _clients.end();
			close(_mainSd);
			while (it != ite){
				close(*it);
				it++;
			}
		}
		sockets&	operator=(const sockets& x){
			_mainSd = x._mainSd;
			_nsds = x._nsds;
			_clients = x._clients;
			_port = x._port;
			_address = x._address;
			return *this;
		}
		void	bindSock(){
			//check port if already bound
			_address.sin_addr.s_addr = INADDR_ANY;
			_address.sin_port = htons(_port);
			_address.sin_family = AF_INET;
			if(bind(_mainSd, (SA *)&_address, sizeof(_address)) < 0)
				throw socketErr("bind: ");
		}
		void	listener(int maxLoad) const{
			if(listen(_mainSd, maxLoad) < 0)
				throw socketErr("Listen: ");
			std::cout << "Socket listening on port " << _port << std::endl;
		}
		int		acceptClient(){
			int	newClient, addrlen = sizeof(_address);
			//can loop over accept and check errno if != EWOULDBLOCK
			if((newClient = accept(_mainSd, (SA *)&_address, (socklen_t *)&addrlen)) < 0)
				throw socketErr("accept: ");
			std::cout << "new connection on port " << _port << std::endl;
			_nsds++;
			_clients.push_back(newClient);
			return newClient;
		}
		void	setNonBlock() const{
			//protect system call
			fcntl(_mainSd, F_SETFL, O_NONBLOCK);
		}
		std::vector<int>&	getClientsVec() {return _clients; }
		int					getNumSds() const {return _nsds; }
		int					getMainSock() const {return _mainSd; }
		unsigned short		getPort() const {return _port; }
		struct sockaddr_in	getAddr() const { return _address;}
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
