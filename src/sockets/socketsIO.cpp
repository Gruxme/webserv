/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketsIO.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 10:41:08 by abiari            #+#    #+#             */
/*   Updated: 2022/03/14 18:57:03 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socketsIO.hpp"

socketsIO::socketsIO(): _nfds(0), _socks(), _pollfds() {}

socketsIO::socketsIO(const socketsIO& x) { this->operator=(x); }

socketsIO::~socketsIO() {
	std::vector<sockets *>::iterator it = _socks.begin(), ite = _socks.end();
	while (it != ite)
	{
		delete *it;
		it++;
	}
}

socketsIO&	socketsIO::operator=(const socketsIO& x){
	_nfds = x._nfds;
	_socks = x._socks;
	_pollfds = x._pollfds;
	return *this;
}

sockets* 	socketsIO::find(size_t port){
	std::vector<sockets *>::iterator	it = _socks.begin();
	std::vector<sockets *>::iterator	ite = _socks.end();
	while(it != ite){
		if ((*it)->getConfigs()[0].getPort() == port)
			return *it;
		it++;
	}
	return NULL;
}

void	socketsIO::setSock(sockets *sock){
	struct	pollfd	fds = {};
	_socks.push_back(sock);
	fds.fd = sock->getMainSock();
	fds.events = POLLIN;
	_pollfds.push_back(fds);
	_nfds++;
}

bool	socketsIO::_tryConnect( int fd ){
	bool	wasMainSock = false;
	struct	pollfd	fds = {};

	for (size_t j = 0; j < _socks.size(); j++)
	{
		if (fd != _socks[j]->getMainSock())
			continue;
		wasMainSock = true;
		std::cout << "socket listening on port: " << _socks[j]->getConfigs()[0].getPort() << " is readable, with fd: " << fd << std::endl;
		try
		{
			fds.fd = _socks[j]->acceptClient();
			_requests[fds.fd].setConfigs(_socks[j]->getConfigs());
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
			break ;
		}
		fds.events = POLLIN;
		_nfds++;
		_pollfds.push_back(fds);
	}
	return wasMainSock;
}

void	socketsIO::eventListener()
{
	char buffer[4096];
	int rc;
	bool connClosed;
	bool isErrorResp = false;
	
	while (1)
	{
		connClosed = false;
		std::cout << "Waiting on poll..." << std::endl;
		rc = poll(&_pollfds[0], _nfds, -1);
		for (size_t i = 0 ; i < _pollfds.size() ; i++) {
			std::cout << _pollfds[i].fd << std::endl;
		}
		if (rc < 0)
			throw socketIOErr("poll: ");
		for (int i = 0; i < _nfds; i++)
		{
			if (_pollfds[i].revents == 0)
				continue;
			if (_pollfds[i].revents != POLLIN && _pollfds[i].revents != POLLOUT)
			{
				std::cout << "client with fd: " << _pollfds[i].fd << " closed after client closed it's connection end" << std::endl;
				close(_pollfds[i].fd);
				std::cout << "Error: revents = " << std::hex << _pollfds[i].revents << std::endl << std::dec;
				_pollfds.erase(_pollfds.begin() + i);
				_nfds--;
				continue;
			}

			
			if (!_tryConnect(_pollfds[i].fd))
			{
				if (_pollfds[i].revents == POLLIN)
				{
					std::cout << "POLLIN on: " << _pollfds[i].fd << std::endl;
					bzero(buffer, 4096);
					rc = recv(_pollfds[i].fd, &buffer, sizeof(buffer), 0);
					if (rc == 0 || rc == -1){
						std::cout << "recv from: " << _pollfds[i].fd << " failed. Connection closed and client removed" << std::endl;
						close(_pollfds[i].fd);
						_pollfds.erase(_pollfds.begin() + i);
						_nfds--;
						continue;
					}
					_requests[_pollfds[i].fd].append(&buffer[0], rc);
					std::cout << "fd: " << _pollfds[i].fd << " received: " << rc << " bytes" << std::endl;
					try {
						_requests[_pollfds[i].fd].parse();
						if (g_sigpipe) {
							_requests.erase(_pollfds[i].fd);
							_responses.erase(_pollfds[i].fd);
							std::cout << "client with fd: " << _pollfds[i].fd << " closed and AFTER ERROR IN UPLOAD" << std::endl;
							close(_pollfds[i].fd);
							_pollfds.erase(_pollfds.begin() + i);
							_nfds--;
							isErrorResp = false;
							g_sigpipe = false;
							continue ;
						}
					}
					catch (const std::exception &e) {
						if(_requests[_pollfds[i].fd].getConfig().getLocationClass()[_requests[_pollfds[i].fd].getPos()].getRedirect().empty()){
							_responses[_pollfds[i].fd].setData(_requests.find(_pollfds[i].fd)->second);
							_responses[_pollfds[i].fd].errorMsg(e.what());
							isErrorResp = _responses[_pollfds[i].fd].isError();
							_pollfds[i].events = POLLOUT;
							continue;
						}
					}
					// check if req complete and set event to pollout
					if (_requests[_pollfds[i].fd].isComplete() || isErrorResp)
						_pollfds[i].events = POLLOUT;
					std::cout << "===============REQUEST BEGIN FOR FD: " << _pollfds[i].fd << "===================\n";
					std::cout << _requests[_pollfds[i].fd] << std::endl;
				}
				else if (_pollfds[i].revents == POLLOUT)
				{
					// response	currRes = _responses[_pollfds[i].fd];
					Request		currReq = _requests[_pollfds[i].fd];
					std::cout << "POLLOUT on fd" << _pollfds[i].fd << std::endl;
					std::string content("");
					if(isErrorResp){
						if (_responses[_pollfds[i].fd].getHeaderStatus())
							content = _responses[_pollfds[i].fd].getBodyContent();
						else
						{
							content = _responses[_pollfds[i].fd].getHeaders();
							std::cout << content << std::endl;
						}
						rc = send(_pollfds[i].fd, content.c_str(), content.length(), 0);
						std::cout << "Content lenght of read data " << content.length() << std::endl;
						if (rc >= 0)
						{
							bool headersStatus = _responses[_pollfds[i].fd].getHeaderStatus();
							if (rc < static_cast<int>(content.length()) && headersStatus)
							{
								_responses[_pollfds[i].fd].offsetCursor(rc - content.length()); // check if headers sending might fail and set things accordingly
								if (headersStatus)
									_responses[_pollfds[i].fd].setBytesSent(rc);
							}
							else if (rc == static_cast<int>(content.length()) && !headersStatus)
								_responses[_pollfds[i].fd].headersSent();
							else if (headersStatus)
								_responses[_pollfds[i].fd].setBytesSent(rc);
							if (_responses[_pollfds[i].fd].bodyEof() || g_sigpipe)
							{
								_requests.erase(_pollfds[i].fd);
								_responses.erase(_pollfds[i].fd);
								std::cout << "client with fd: " << _pollfds[i].fd << " closed and deleted after error response sent" << std::endl;
								close(_pollfds[i].fd);
								_pollfds.erase(_pollfds.begin() + i);
								_nfds--;
								isErrorResp = false;
								g_sigpipe = false;
							}
							continue;
						}
					}
					bool connClose = currReq.getHeaders().find("Connection")->second == "close";
					_responses[_pollfds[i].fd].setData(currReq);
					if(!_responses[_pollfds[i].fd].getHeaderStatus())
						_responses[_pollfds[i].fd].serveRequest();
					if(_responses[_pollfds[i].fd].getHeaderStatus() && !_responses[_pollfds[i].fd].isCgi())
						if(_responses[_pollfds[i].fd].isAutoIndex())
							content = _responses[_pollfds[i].fd].indexListContent();
						else
							content = _responses[_pollfds[i].fd].getBodyContent();
					else if (_responses[_pollfds[i].fd].isCgi()){
						content = _responses[_pollfds[i].fd].getCgi().getContent();
						std::cout << content << std::endl;
					}
					else {
						content = _responses[_pollfds[i].fd].getHeaders();
						std::cout << content << std::endl;
					}
					rc = send(_pollfds[i].fd, content.c_str(), content.length(), 0);
					std::cout << "Content lenght of read data  " << content.length() << std::endl;
					if(rc >= 0)
					{
						if (rc < static_cast<int>(content.length()) && _responses[_pollfds[i].fd].getHeaderStatus()){
							_responses[_pollfds[i].fd].offsetCursor(rc - content.length()); // check if headers sending might fail and set things accordingly
							if(_responses[_pollfds[i].fd].getHeaderStatus())
								_responses[_pollfds[i].fd].setBytesSent(rc);
						}
						else if (rc == static_cast<int>(content.length()) && !_responses[_pollfds[i].fd].getHeaderStatus())
							_responses[_pollfds[i].fd].headersSent();
						else if(_responses[_pollfds[i].fd].getHeaderStatus() && !_responses[_pollfds[i].fd].isCgi()){
							_responses[_pollfds[i].fd].setBytesSent(rc);
						}
						if(_responses[_pollfds[i].fd].isRedirect()){
							std::cout << "client with fd: " << _pollfds[i].fd << " closed and deleted after redirect to:  " 
									<< _requests[_pollfds[i].fd].getConfig().getLocationClass()[_requests[_pollfds[i].fd].getPos()].getRedirect() << std::endl;
							_requests.erase(_pollfds[i].fd);
							_responses.erase(_pollfds[i].fd);
							_pollfds.erase(_pollfds.begin() + i);
							_nfds--;
							continue ;
						}
						else if (_responses[_pollfds[i].fd].bodyEof() || g_sigpipe || _responses[_pollfds[i].fd].isCgi())
						{
							std::cout << "client with fd: " << _pollfds[i].fd << " kept alive and reset to POLLIN" << std::endl;
							// _requests.erase(_pollfds[i].fd));
							_requests[_pollfds[i].fd].reset();
							_responses.erase(_pollfds[i].fd);
							_pollfds[i].events = POLLIN;
							connClosed = true;
						}
					}
					else{
						std::cout << "client with fd: " << _pollfds[i].fd << " closed and deleted after send error" << std::endl;
						close(_pollfds[i].fd);
						_requests.erase(_pollfds[i].fd);
						_responses.erase(_pollfds[i].fd);
						_pollfds.erase(_pollfds.begin() + i);
						_nfds--;
						continue ;
					}
					if ((connClosed && connClose) || isErrorResp)
					{
						std::cout << "client with fd: " << _pollfds[i].fd << " closed and deleted after being served and Connection set to Close" << std::endl;
						close(_pollfds[i].fd);
						_requests.erase(_pollfds[i].fd);
						_responses.erase(_pollfds[i].fd);
						_pollfds.erase(_pollfds.begin() + i);
						_nfds--;
					}
				}
			}
		}
	}
}
