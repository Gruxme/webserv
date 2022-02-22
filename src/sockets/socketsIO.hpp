/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketsIO.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 10:41:08 by abiari            #+#    #+#             */
/*   Updated: 2022/02/22 15:50:17 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "sockets.hpp"
#include "../request/Request.hpp"
#include "../response/response.hpp"
#include <poll.h>
#include <memory>
#include <map>

class socketsIO
{
	public:
		socketsIO();
		socketsIO(const socketsIO& x);
		~socketsIO();
		socketsIO&	operator=(const socketsIO& x);
		void	setSock(const sockets& sock);
		void	recvData();
		void	sendData();
		void	eventListener( void );

		class socketIOErr: public std::exception{
			public:
				explicit socketIOErr(const std::string& errStr) throw() : _errStr(errStr) {}
				~socketIOErr() throw(){}
				virtual const char *what() const throw(){
					perror(_errStr.c_str());
					return "";
				}
			private:
				std::string	_errStr;
		};
	private:
			bool	_tryConnect( int fd );
	private:
		int							_nfds;
		std::allocator<sockets>		_socksAlloc;
		std::vector<sockets *>		_socks;
		std::vector<struct pollfd>	_pollfds;
		std::map<int, Request>		_requests; // make a werbserv class with configfile, request and response objs, and a corresponding fd instead of this line
};
