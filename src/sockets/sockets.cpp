/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/03/02 17:57:47 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sockets.hpp"

sockets::sockets(): _mainSd(-1), _nsds(0), _clients(), _config(), _address(){}

sockets::sockets(ServerConfigClass conf, int maxLoad): _clients(), _config(conf), _address(){
	int	opt = 1;
	if((_mainSd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw socketErr("socket: ");
	if(setsockopt(_mainSd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw socketErr("setsockopt: ");
	if (fcntl(_mainSd, F_SETFL, O_NONBLOCK) < 0)
		throw socketErr("fcntl: ");
	_bindSock();
	_listener(maxLoad);
	_nsds = 1;
}

sockets::sockets(const sockets& x) : _mainSd(x._mainSd), _nsds(x._nsds), _config(x._config), _address(x._address){}

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
	_config = x._config;
	_address = x._address;
	return *this;
}

void	sockets::_bindSock(){
	//check port if already bound
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(_config.getPort());
	_address.sin_family = AF_INET;
	if(bind(_mainSd, (SA *)&_address, sizeof(_address)) < 0)
		throw socketErr("bind: ");
}

void	sockets::_listener(int maxLoad){
	if(listen(_mainSd, maxLoad) < 0)
		throw socketErr("Listen: ");
	std::cout << "Socket listening on port " << _config.getPort() << std::endl;
}

int		sockets::acceptClient(){
	int	newClient, addrlen = sizeof(_address);
	//can loop over accept and check errno if != EWOULDBLOCK
	if((newClient = accept(_mainSd, (SA *)&_address, (socklen_t *)&addrlen)) < 0)
		throw socketErr("accept: ");
	std::cout << "new connection on port " << _config.getPort() << std::endl;
	_nsds++;
	_clients.push_back(newClient);
	return newClient;
}

std::vector<int>&	sockets::getClientsVec() { return _clients; }
int					sockets::getNumSds() const { return _nsds; }
int					sockets::getMainSock() const { return _mainSd; }
ServerConfigClass		sockets::getConfig() const { return _config; }
struct sockaddr_in	sockets::getAddr() const { return _address; }
