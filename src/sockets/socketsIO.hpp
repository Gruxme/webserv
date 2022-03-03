/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketsIO.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 10:41:08 by abiari            #+#    #+#             */
/*   Updated: 2022/03/02 18:36:40 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "sockets.hpp"
#include "../request/Request.hpp"
#include "../response/response.hpp"
#include <poll.h>
#include <memory>
#include <map>
extern bool	g_sigpipe;

class socketsIO
{
	public:
		socketsIO();
		socketsIO(const socketsIO& x);
		~socketsIO();
		socketsIO&	operator=(const socketsIO& x);
		void	setSock(sockets *sock);
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
			// int		_recvData(int client);
			// void	_sendData();
	private:
		int								_nfds;
		std::vector<sockets *>			_socks;
		std::vector<struct pollfd>		_pollfds;
		std::map<int, Request>			_requests;
		std::map<int, response>			_responses;
		//std::map<int, std::pair<Request, response> >	_reqRes;
};
