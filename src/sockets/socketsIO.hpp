/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketsIO.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 10:41:08 by abiari            #+#    #+#             */
/*   Updated: 2022/02/14 15:53:44 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "sockets.hpp"
#include "Request.hpp"
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
		void	eventListener();

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
		int							_nfds;
		std::allocator<sockets>		_socksAlloc;
		std::vector<sockets *>		_socks;
		std::vector<struct pollfd>	_pollfds;
		std::map<int,Request>	_requests;
};
