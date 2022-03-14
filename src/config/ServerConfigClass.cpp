/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigClass.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:45:06 by aabounak          #+#    #+#             */
/*   Updated: 2022/03/14 16:41:40 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerConfigClass.hpp"

ServerConfigClass::ServerConfigClass() :
    _port(-1),
    _serverName(""),
    _root(""),
    _redirect(""),
    _index(""),
    _bodySizeLimit(10240),
    _accessLog(""),
    _errorPage(""),
    _locationCount(0),
    _location(0) {}

ServerConfigClass::ServerConfigClass( ServerConfigClass const &x ) {
    *this = x;
}

ServerConfigClass& ServerConfigClass::operator=( const ServerConfigClass& rhs ) {
    if (this != &rhs) {
        this->_port = rhs._port;
        this->_serverName = rhs._serverName;
        this->_root = rhs._root;
        this->_redirect = rhs._redirect;
        this->_index = rhs._index;
        this->_bodySizeLimit = rhs._bodySizeLimit;
        this->_accessLog = rhs._accessLog;
        this->_errorPage = rhs._errorPage;
        this->_locationCount = rhs._locationCount;
        this->_location = rhs._location;
    }
    return *this;
}

ServerConfigClass::~ServerConfigClass() {}

size_t   ServerConfigClass::getPort() const { return this->_port; }
std::string     ServerConfigClass::getServerName() const { return this->_serverName; }
std::string     ServerConfigClass::getRoot() const { return this->_root ;}
std::string     ServerConfigClass::getRedirect() const { return this->_redirect ;}
std::string     ServerConfigClass::getIndex() const { return this->_index ;}
size_t   ServerConfigClass::getBodySizeLimit() const { return this->_bodySizeLimit; }
std::string     ServerConfigClass::getAccessLog() const { return this->_accessLog; }
std::string     ServerConfigClass::getErrorPage() const { return this->_errorPage; }
size_t   ServerConfigClass::getLocationCount() const { return this->_locationCount; }
std::vector<LocationClass>  ServerConfigClass::getLocationClass() const { return this->_location; }


