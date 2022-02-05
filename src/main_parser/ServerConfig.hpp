/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 14:53:26 by aabounak          #+#    #+#             */
/*   Updated: 2022/02/05 18:26:38 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "../TempHeader.hpp"
# include "LocationClass.hpp"

# define __AUTOINDEX_OFF__ 0
# define __AUTOINDEX_ON__ 1

namespace ft {
    class ServerConfig {
        /* ----- PRIVATE ----- */
        public:
            __SIZE_TYPE__   __port;
            std::string     __serverName;
            std::string     __host;
            __SIZE_TYPE__   __bodySizeLimit;
            std::string     __accessLog;
            std::string     __errorPage;
            bool            __autoindex;
            
            __SIZE_TYPE__   __locationCount;
            LocationClass * __location;
            
        public:
            /* ----- Constructors & Destructor respectively ----- */
            ServerConfig() :
                __port(8080),
                __serverName(""),
                __host(""),
                __bodySizeLimit(0),
                __accessLog(""),
                __errorPage(""),
                __autoindex(__AUTOINDEX_OFF__),
                __locationCount(0) {}
            ServerConfig( ServerConfig const &x ) { *this = x; }
            ServerConfig& operator= ( const ServerConfig& rhs ) {
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
            ~ServerConfig() {}

            /* ----- Getters ----- */
            __SIZE_TYPE__   getPort() const { return this->__port; }
            std::string     getServerName() const { return this->__serverName; }
            std::string     getHost() const { return this->__host; }
            __SIZE_TYPE__   getBodySizeLimit() const { return this->__bodySizeLimit; }
            std::string     getAccessLog() const { return this->__accessLog; }
            std::string     getErrorPage() const { return this->__errorPage; }
            bool            getAutoIndex() const { return this->__autoindex; }
            __SIZE_TYPE__   getLocationCount() const { return this->__locationCount; }

            
    };
}