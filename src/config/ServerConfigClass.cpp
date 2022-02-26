/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigClass.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:45:06 by aabounak          #+#    #+#             */
/*   Updated: 2022/02/23 10:57:12 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerConfigClass.hpp"

/* ----- Constructors & Destructor respectively ----- */
ServerConfigClass::ServerConfigClass() :
    _port(8080),
    _serverName(""),
    _root(""),
    _bodySizeLimit(0),
    _accessLog(""),
    _errorPage("../errorPages/"),
    _autoindex(_AUTOINDEX_OFF_),
    _locationCount(0),
    _location(0) {}

ServerConfigClass::ServerConfigClass( ServerConfigClass const &x ) {
    *this = x;
}

ServerConfigClass& ServerConfigClass::operator= ( const ServerConfigClass& rhs ) {
    if (this != &rhs) {
        this->_port = rhs._port;
        this->_serverName = rhs._serverName;
        this->_root = rhs._root;
        this->_bodySizeLimit = rhs._bodySizeLimit;
        this->_accessLog = rhs._accessLog;
        this->_errorPage = rhs._errorPage;
        this->_autoindex = rhs._autoindex;
        this->_locationCount = rhs._locationCount;
        /* -- {DEEP COPY} _serverConf */
        this->_location = rhs._location;
    }
    return *this;
}

ServerConfigClass::~ServerConfigClass() {}

/* ----- Getters ----- */
size_t   ServerConfigClass::getPort() const { return this->_port; }
std::string     ServerConfigClass::getServerName() const { return this->_serverName; }
std::string     ServerConfigClass::getRoot() const { return this->_root; }
size_t   ServerConfigClass::getBodySizeLimit() const { return this->_bodySizeLimit; }
std::string     ServerConfigClass::getAccessLog() const { return this->_accessLog; }
std::string     ServerConfigClass::getErrorPage() const { return this->_errorPage; }
bool            ServerConfigClass::getAutoIndex() const { return this->_autoindex; }
size_t   ServerConfigClass::getLocationCount() const { return this->_locationCount; }
std::vector<LocationClass>  ServerConfigClass::getLocationClass() const { return this->_location; }
