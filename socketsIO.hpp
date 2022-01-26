/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketsIO.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 10:41:08 by abiari            #+#    #+#             */
/*   Updated: 2022/01/13 17:35:01 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sockets.hpp"
#include <poll.h>
#include <memory>

class socketsIO
{
	public:
		socketsIO(): _nfds(0), _socksAlloc(), _socks(), _pollfds(){
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
			_nfds = x._nfds;
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
			_nfds++;
			// FD_SET(newSock->getMainSock(), &_readSocks);
			while (it != ite){
				fds.fd = *it;
				fds.events = POLLIN | POLLOUT;
				_pollfds.push_back(fds);
				_nfds++;
				// FD_SET(*it, &_readSocks);
				// FD_SET(*it, &_writeSocks);
				it++;
			}
		}
		void	fixPollfd(){
			
		}
		void	eventListener(){
			std::vector<char>	buffer(1024);
			std::string			req;
			struct	pollfd	fds;
			int				wasMainSock = 0;
			int		rc = -1;
			bool	endServer = false/* , closeConn = false */;
			while(endServer == false) {
				wasMainSock = 0;
				std::cout << "Waiting on poll..." << std::endl;
				rc = poll(&_pollfds[0], _nfds, -1);
				if (rc < 0){
					throw socketIOErr("poll: ");
					break ;
				}
				if (rc == 0){
					throw socketIOErr("poll: ");
					break ;
				}
				for (int i = 0; i < _nfds; i++) {
					if (_pollfds[i].revents == 0)
						continue;
					if (_pollfds[i].revents != POLLIN && _pollfds[i].revents != POLLOUT && _pollfds[i].revents != (POLLIN | POLLOUT))
					{
						std::cout << "Error: revents = " << std::hex <<_pollfds[i].revents << std::endl; 
						endServer = true;
						break;
					}
					for(size_t j = 0; j < _socks.size(); j++){
						if(_pollfds[i].fd != _socks[j]->getMainSock())
							continue ;
						wasMainSock = 1;
						std::cout << "socket listening on port: " << _socks[j]->getPort() << " is readable" << std::endl;
						try {
							fds.fd = _socks[j]->acceptClient();
							fds.events = POLLIN | POLLOUT;
							_nfds++;
							_pollfds.push_back(fds);
						}
						catch(const std::exception& e) {
							std::cerr << e.what() << '\n';
							endServer = true;
							break ;
						}
					}
					if(!wasMainSock){
						recv(_pollfds[i].fd, &buffer[0], buffer.size(), 0);
						req.append(&buffer[0]);
						std::cout << "===============REQUEST BEGIN===================\n";
						std::cout << req << std::endl;
						std::string res("HTTP/1.1 200 OK\nConnection: close\nContent-Type: text/html\nContent-Length: 306\n\n<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"https://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\">\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n<title>Webserv</title>\n<body>\n<h1>Connected to server</h1>\n</body>\n</html>");
						send(_pollfds[i].fd, res.c_str(), res.length(), 0);
						close(_pollfds[i].fd);
						_pollfds.erase(_pollfds.begin() + i);
						_nfds--;
						//need request object to append buffer into
						//need a map with fd as key and body of request as value (check mouad if you forgot details)
					}
				}
			}
		}
		class socketIOErr: public std::exception{
			public:
				socketIOErr(std::string errStr) throw() : _errStr(errStr) {}
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
		// fd_set						_readSocks, _writeSocks;
};
