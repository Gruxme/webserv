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
    __port(8080),
    __serverName(""),
    __root(""),
    __bodySizeLimit(0),
    __accessLog(""),
    __errorPage(""),
    __autoindex(__AUTOINDEX_OFF__),
    __locationCount(0) {}

ServerConfigClass::ServerConfigClass( ServerConfigClass const &x ) {
    *this = x;
}

ServerConfigClass& ServerConfigClass::operator= ( const ServerConfigClass& rhs ) {
    if (this != &rhs) {
        this->__port = rhs.__port;
        this->__serverName = rhs.__serverName;
        this->__root = rhs.__root;
        this->__bodySizeLimit = rhs.__bodySizeLimit;
        this->__accessLog = rhs.__accessLog;
        this->__errorPage = rhs.__errorPage;
        this->__autoindex = rhs.__autoindex;
        this->__locationCount = rhs.__locationCount;
        /* -- {DEEP COPY} __serverConf */
    }
    return *this;
}

ServerConfigClass::~ServerConfigClass() {}

/* ----- Getters ----- */
size_t   ServerConfigClass::getPort() const { return this->__port; }
std::string     ServerConfigClass::getServerName() const { return this->__serverName; }
std::string     ServerConfigClass::getRoot() const { return this->__root; }
size_t   ServerConfigClass::getBodySizeLimit() const { return this->__bodySizeLimit; }
std::string     ServerConfigClass::getAccessLog() const { return this->__accessLog; }
std::string     ServerConfigClass::getErrorPage() const { return this->__errorPage; }
bool            ServerConfigClass::getAutoIndex() const { return this->__autoindex; }
size_t   ServerConfigClass::getLocationCount() const { return this->__locationCount; }
LocationClass * ServerConfigClass::getLocation() const { return this->__location; }
