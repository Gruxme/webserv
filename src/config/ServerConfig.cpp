/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:45:06 by aabounak          #+#    #+#             */
/*   Updated: 2022/02/21 17:36:49 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerConfig.hpp"

/* ----- Constructors & Destructor respectively ----- */
ServerConfig::ServerConfig() :
    __port(8080),
    __serverName(""),
    __host(""),
    __bodySizeLimit(0),
    __accessLog(""),
    __errorPage(""),
    __autoindex(__AUTOINDEX_OFF__),
    __locationCount(0) {}

ServerConfig::ServerConfig( ServerConfig const &x ) {
    *this = x;
}

ServerConfig& ServerConfig::operator= ( const ServerConfig& rhs ) {
    if (this != &rhs) {
        this->__port = rhs.__port;
        this->__serverName = rhs.__serverName;
        this->__host = rhs.__host;
        this->__bodySizeLimit = rhs.__bodySizeLimit;
        this->__accessLog = rhs.__accessLog;
        this->__errorPage = rhs.__errorPage;
        this->__autoindex = rhs.__autoindex;
        this->__locationCount = rhs.__locationCount;
        // this->__location = rhs.__location;
    }
    return *this;
}

ServerConfig::~ServerConfig() {}

/* ----- Getters ----- */
size_t   ServerConfig::getPort() const { return this->__port; }
std::string     ServerConfig::getServerName() const { return this->__serverName; }
std::string     ServerConfig::getHost() const { return this->__host; }
size_t   ServerConfig::getBodySizeLimit() const { return this->__bodySizeLimit; }
std::string     ServerConfig::getAccessLog() const { return this->__accessLog; }
std::string     ServerConfig::getErrorPage() const { return this->__errorPage; }
bool            ServerConfig::getAutoIndex() const { return this->__autoindex; }
size_t   ServerConfig::getLocationCount() const { return this->__locationCount; }
