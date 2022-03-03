/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketsIO.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 10:41:08 by abiari            #+#    #+#             */
/*   Updated: 2022/03/03 21:05:16 by abiari           ###   ########.fr       */
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
		std::cout << "socket listening on port: " << _socks[j]->getConfig().getPort() << " is readable, with fd: " << fd << std::endl;
		try
		{
			fds.fd = _socks[j]->acceptClient();
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
	unsigned long sentBytes = 0;
	bool connClosed;
	while (1)
	{
		connClosed = false;
		std::cout << "Waiting on poll..." << std::endl;
		rc = poll(&_pollfds[0], _nfds, -1);
		if (rc < 0)
			throw socketIOErr("poll: ");
		for (int i = 0; i < _nfds; i++)
		{
			if (_pollfds[i].revents == 0)
				continue;
			if (_pollfds[i].revents != POLLIN && _pollfds[i].revents != POLLOUT)
			{
				close(_pollfds[i].fd);
				_pollfds.erase(_pollfds.begin() + i);
				_nfds--;
				std::cout << "Error: revents = " << std::hex << _pollfds[i].revents << std::endl << std::dec;
				continue;
			}
			if (!_tryConnect(_pollfds[i].fd))
			{
				if (_pollfds[i].revents == POLLIN)
				{
					std::cout << "POLLIN on: " << _pollfds[i].fd << std::endl;
					bzero(buffer, 4096);
					rc = recv(_pollfds[i].fd, &buffer, sizeof(buffer), 0);
					if (rc == -1)
						continue;
					if (rc == 0){
						close(_pollfds[i].fd);
						_pollfds.erase(_pollfds.begin() + i);
						_nfds--;
						continue ; // still not sure of this
					}
					_requests[_pollfds[i].fd].append(&buffer[0]);
					std::cout << "fd: " << _pollfds[i].fd << "received: " << rc << " bytes" << std::endl;
					try {
						_requests[_pollfds[i].fd].parse();
					}
					catch (const std::exception &e){
						_responses[_pollfds[i].fd].errorMsg(e.what());
					}
					// check if req complete and set event to pollout
					if (_requests[_pollfds[i].fd].isComplete())
						_pollfds[i].events = POLLOUT;
					std::cout << "===============REQUEST BEGIN FOR FD: " << _pollfds[i].fd << "===================\n";
					std::cout << _requests[_pollfds[i].fd] << std::endl;
				}
				else if (_pollfds[i].revents == POLLOUT)
				{
					std::cout << "POLLOUT on fd" << _pollfds[i].fd << std::endl;
					std::string content("");
					bool connClose = _requests[_pollfds[i].fd].getHeaders().find("Connection")->second == "close";
					bool isErrorResp = _responses[_pollfds[i].fd].isError();
					if(_requests.find(_pollfds[i].fd)->second.getPort() == 0)
						_responses[_pollfds[i].fd].setData(_socks[0]->getConfig(), _requests.find(_pollfds[i].fd)->second);
					else {
						for (size_t j = 0; j < _socks.size(); j++)
							if (_socks[j]->getConfig().getPort() == _requests.find(_pollfds[i].fd)->second.getPort())
								_responses[_pollfds[i].fd].setData(_socks[j]->getConfig(), _requests.find(_pollfds[i].fd)->second);
					}
					if(!_responses[_pollfds[i].fd].isError() && !_responses[_pollfds[i].fd].getHeaderStatus())
						_responses[_pollfds[i].fd].serveRequest();
					// std::cout << "Frist part" << std::endl;
					//send chunked files as one request with continuous body sent over poll loops with one content lenght header instead of encoding literal chunks
					if(_responses[_pollfds[i].fd].getHeaderStatus())
						content = _responses[_pollfds[i].fd].getBodyContent();
					else {
						content = _responses[_pollfds[i].fd].getHeaders();
						std::cout << content << std::endl;
					}
					rc = send(_pollfds[i].fd, content.c_str(), content.length(), 0);
					std::cout << "Content lenght" << content.length() << std::endl;
					if(rc >= 0)
					{
						if (rc < static_cast<int>(content.length()) && _responses[_pollfds[i].fd].getHeaderStatus()){
							_responses[_pollfds[i].fd].offsetCursor(rc - content.length()); // check if headers sending might fail and set things accordingly
							if(_responses[_pollfds[i].fd].getHeaderStatus())
								_responses[_pollfds[i].fd].setBytesSent(rc);
						}
						else if (rc == static_cast<int>(content.length()) && !_responses[_pollfds[i].fd].getHeaderStatus())
							_responses[_pollfds[i].fd].headersSent();
						else if(_responses[_pollfds[i].fd].getHeaderStatus())
							_responses[_pollfds[i].fd].setBytesSent(rc);
						if (_responses[_pollfds[i].fd].bodyEof() || g_sigpipe)
						{
							_requests.erase(_pollfds[i].fd);
							_responses.erase(_pollfds[i].fd);
							_pollfds[i].events = POLLIN;
							connClosed = true;
						}
					}
					else{
						close(_pollfds[i].fd);
						_pollfds.erase(_pollfds.begin() + i);
						_nfds--;
						continue ;
					}
					std::cout << "sent: " << rc << " bytes"
							  << " for a total of " << sentBytes << " bytes" << std::endl;
					
					if ((connClosed && connClose) || isErrorResp)
					{
						close(_pollfds[i].fd);
						_pollfds.erase(_pollfds.begin() + i);
						_nfds--;
					}
				}
			}
		}
	}
}
