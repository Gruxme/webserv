/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketsIO.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 10:41:08 by abiari            #+#    #+#             */
/*   Updated: 2021/12/24 16:54:18 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sockets.hpp"
// #include <sys/select.h>
#include <poll.h>
#include <memory>

class socketsIO
{
	public:
		socketsIO(): _socksAlloc(), _socks(), _pollfds(){
			// FD_ZERO(&_readSocks);
			// FD_ZERO(&_writeSocks);
		}
		socketsIO(const socketsIO& x) {this->operator=(x);}
		~socketsIO(){
			std::vector<sockets *>::iterator it = _socks.begin(), ite = _socks.end();
			while (it != ite){
				_socksAlloc.destroy(*it);
				_socksAlloc.deallocate(*it, 1);
				it++;
			}
		}
		socketsIO&	operator=(const socketsIO& x){
			_socksAlloc = x._socksAlloc;
			_socks = x._socks;
			_pollfds = x._pollfds;
			// _readSocks = x._readSocks;
			// _writeSocks = x._writeSocks;
			return *this;
		}
		void	setSock(const sockets& sock){
			struct	pollfd	fds;
			sockets *newSock = _socksAlloc.allocate(1);
			_socksAlloc.construct(newSock, sock);
			std::vector<int>::iterator it = newSock->getClientsVec().begin();
			std::vector<int>::iterator ite = newSock->getClientsVec().end();
			_socks.push_back(newSock);
			fds.fd = newSock->getMainSock();
			fds.events = POLLIN;
			_pollfds.push_back(fds);
			// FD_SET(newSock->getMainSock(), &_readSocks);
			while (it != ite){
				fds.fd = *it;
				fds.events = POLLIN | POLLOUT;
				_pollfds.push_back(fds);
				// FD_SET(*it, &_readSocks);
				// FD_SET(*it, &_writeSocks);
				it++;
			}
		}
		void	eventListener(){
			
		}
	private:
		std::allocator<sockets>		_socksAlloc;
		std::vector<sockets *>		_socks;
		std::vector<struct pollfd>	_pollfds;
		// fd_set						_readSocks, _writeSocks;
};
