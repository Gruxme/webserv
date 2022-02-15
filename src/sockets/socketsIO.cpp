/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketsIO.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 10:41:08 by abiari            #+#    #+#             */
/*   Updated: 2022/02/15 10:21:28 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socketsIO.hpp"

socketsIO::socketsIO(): _nfds(0), _socksAlloc(), _socks(), _pollfds() {}

socketsIO::socketsIO(const socketsIO& x): _nfds(0) { this->operator=(x); }

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
	std::vector<int>::iterator it = newSock->getClientsVec().begin();
	std::vector<int>::iterator ite = newSock->getClientsVec().end();
	_socks.push_back(newSock);
	fds.fd = newSock->getMainSock();
	fds.events = POLLIN;
	_pollfds.push_back(fds);
	_nfds++;
	while (it != ite){
		fds.fd = *it;
		fds.events = POLLIN | POLLOUT;
		_pollfds.push_back(fds);
		_nfds++;
		it++;
	}
}

void	socketsIO::eventListener(){
	struct	pollfd	fds = {};
	char				buffer[1024];
	Request				req;
	int					wasMainSock;
	int					rc;
	unsigned long		sentBytes = 0;
	bool				endServer = false/* , closeConn = false */;
	bool				connClosed = false;
	while(!endServer) {
		wasMainSock = 0;
		std::cout << "Waiting on poll..." << std::endl;
		rc = poll(&_pollfds[0], _nfds, -1);
		if (rc < 0)
			throw socketIOErr("poll: ");
		if (rc == 0)
			throw socketIOErr("poll: ");
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
				//to be refactored -> split recv and send, recv then check if request is complete before forging and sending a response
				std::string res("HTTP/1.1 200 OK\nConnection: close\nContent-Type: text/html\nContent-Length: 741\n\n<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"https://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\">\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n<title>Webserv</title>\n<body>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n<h1>Connected to server</h1>\n</body>\n</html>");
				bzero(buffer, 1024);
				rc = recv(_pollfds[i].fd, &buffer, sizeof(buffer), 0);
				if(rc == -1)
					continue ;
				_requests[_pollfds[i].fd].append(&buffer[0]);
				// req.append(buffer);
				if (rc == 0)
					connClosed = true;
				if (req.isComplete())
					// unchunk body if chunked, make request and send it
					//then check if connection to be closed or kept alive before closing fd
				std::cout << "received: " << rc << "bytes" << std::endl;
				std::cout << "===============REQUEST BEGIN===================\n";
				std::cout << req << std::endl;
				connClosed = false;
				rc = send(_pollfds[i].fd, res.c_str() + sentBytes, res.length() - sentBytes, 0);
				sentBytes += rc;
				std::cout << "sent: " << rc << "bytes"
						  << "for a total of " << sentBytes << "bytes" << std::endl;
				if (sentBytes == res.length())
					connClosed = true;

				if (connClosed) {
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
