/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/03/12 16:08:23 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sockets.hpp"


sockets::sockets(): _mainSd(-1), _nsds(0), _clients(), _configs(), _address(){}

sockets::sockets(size_t port, int maxLoad): _clients(), _configs(), _address(){
	int	opt = 1;
	if((_mainSd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw socketErr("socket: ");
	if(setsockopt(_mainSd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw socketErr("setsockopt: ");
	if (fcntl(_mainSd, F_SETFL, O_NONBLOCK) < 0)
		throw socketErr("fcntl: ");
	try {
		_bindSock(port);
	} catch ( std::exception &e ) {
		std::cout << e.what() << std::endl;
	}
	_listener(maxLoad);
	std::cout << "Socket listening on port " << port << std::endl;
	_nsds = 1;
}

sockets::sockets(const sockets& x) {
	*this = x;
}

sockets::~sockets(){
	std::vector<int>::iterator it = _clients.begin(), ite = _clients.end();
	close(_mainSd);
	while (it != ite){
		close(*it);
		it++;
	}
}

sockets&	sockets::operator=(const sockets& x){
	_mainSd = x._mainSd;
	_nsds = x._nsds;
	_clients = x._clients;
	_configs = x._configs;
	_address = x._address;
	return *this;
}

void	sockets::setConfig(ServerConfigClass conf){
	bool found = false;
	for (size_t i = 0; i < _configs.size(); i++)
	{
		if (_configs[i].getServerName() == conf.getServerName())
			found = true;
	}
	if (!found)
		_configs.push_back(conf);
}

void	sockets::_bindSock(size_t port){
	//check port if already bound
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(port);
	_address.sin_family = AF_INET;
	if (bind(_mainSd, (SA *)&_address, sizeof(_address)) < 0)
		throw socketErr("bind");
}

void	sockets::_listener(int maxLoad){
	if(listen(_mainSd, maxLoad) < 0)
		throw socketErr("Listen: ");
}

int		sockets::acceptClient(){
	int	newClient, addrlen = sizeof(_address);
	//can loop over accept and check errno if != EWOULDBLOCK
	if((newClient = accept(_mainSd, (SA *)&_address, (socklen_t *)&addrlen)) < 0)
		throw socketErr("accept: ");
	std::cout << "new connection on port " << _configs[0].getPort() << std::endl;
	_nsds++;
	_clients.push_back(newClient);
	return newClient;
}

std::vector<int>&				sockets::getClientsVec() { return _clients; }
int								sockets::getNumSds() const { return _nsds; }
int								sockets::getMainSock() const { return _mainSd; }
std::vector<ServerConfigClass>	sockets::getConfigs() const { return _configs; }
struct sockaddr_in				sockets::getAddr() const { return _address; }
