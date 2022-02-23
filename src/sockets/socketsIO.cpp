/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketsIO.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 10:41:08 by abiari            #+#    #+#             */
/*   Updated: 2022/02/23 10:03:57 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socketsIO.hpp"

socketsIO::socketsIO(): _nfds(0), _socksAlloc(), _socks(), _pollfds() {}

socketsIO::socketsIO(const socketsIO& x) { this->operator=(x); }

socketsIO::~socketsIO(){
	std::vector<sockets *>::iterator it = _socks.begin();
	std::vector<sockets *>::iterator ite = _socks.end();
	while (it != ite){
		_socksAlloc.destroy(*it);
		_socksAlloc.deallocate(*it, 1);
		it++;
	}
}

socketsIO&	socketsIO::operator=(const socketsIO& x){
	_nfds = x._nfds;
	_socksAlloc = x._socksAlloc;
	_socks = x._socks;
	_pollfds = x._pollfds;
	return *this;
}

void	socketsIO::setSock(const sockets& sock){
	struct	pollfd	fds = {};
	sockets *newSock = _socksAlloc.allocate(1);
	_socksAlloc.construct(newSock, sock);
	_socks.push_back(newSock);
	fds.fd = newSock->getMainSock();
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
		std::cout << "socket listening on port: " << _socks[j]->getConfig().getPort() << " is readable" << std::endl;
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
	char buffer[1024];
	Request		req;
	response	res;
	int rc;
	unsigned long sentBytes = 0;
	bool connClosed = false;
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
				// close only the fd and not the server loop
				close(_pollfds[i].fd);
				_pollfds.erase(_pollfds.begin() + i);
				_nfds--;
				std::cout << "Error: revents = " << std::hex << _pollfds[i].revents << std::endl;
				continue;
			}
			// std::string res("HTTP/1.1 200 OK\nConnection: close\nContent-Type: text/html\nContent-Length: 741\n\n<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"https://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\">\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n<title>Webserv</title>\n<body>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n</body>\n</html>");
			if (!_tryConnect(_pollfds[i].fd))
			{
				if (!req.isComplete() && _pollfds[i].revents == POLLIN)
				{
					bzero(buffer, 1024);
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
					std::cout << "received: " << rc << "bytes" << std::endl;
					try{
						req.parse();
					}
					catch (const std::exception &e){
						//make method in response class that makes generic error responses
						std::cerr << e.what() << '\n'; // if caught exception forge appropriate err res and continue poll loop to send it
					}
					// check if req complete and set event to pollout
					if (req.isComplete())
						_pollfds[i].events = POLLOUT;
					std::cout << "===============REQUEST BEGIN===================\n";
					std::cout << req << std::endl;
				}
				if (req.isComplete() && _pollfds[i].revents == POLLOUT)
				{
					for (int i = 0; i < _socks.size(); i++)
						if (_socks[i]->getConfig().getPort() == _requests.find(_pollfds[i].fd)->second.getPort())
							res.serveRequest(_socks[i]->getConfig(), _requests.find(_pollfds[i].fd)->second);
					//send chunked files as one request with continuous body sent over poll loops with one content lenght header instead of encoding literal chunks
					rc = send(_pollfds[i].fd, res.getMsg().c_str() + sentBytes, res.getMsg().length() - sentBytes, 0);
					sentBytes += rc;
					std::cout << "sent: " << rc << "bytes"
							  << "for a total of " << sentBytes << "bytes" << std::endl;
					if (sentBytes == res.getMsg().length())
					{
						_requests.erase(_pollfds[i].fd);
						_responses.erase(_pollfds[i].fd);
						connClosed = true;
					}
					if (connClosed && req.getHeaders().find("Connection")->second == "close")
					{
						// check if rc == 0 should close connection regardless
						close(_pollfds[i].fd);
						_pollfds.erase(_pollfds.begin() + i);
						_nfds--;
						connClosed = false;
					}
					// make sure to have the exact Content lenght filled in header
				}
			}
		}
	}
}
