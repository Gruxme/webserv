/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 14:53:26 by aabounak          #+#    #+#             */
/*   Updated: 2022/01/20 21:58:58 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "header.hpp"
# include "LocationClass.hpp"

#include <fstream>


namespace ft {
    class ServerConfig {
        /* These will become private */
        public:
            __SIZE_TYPE__   __port;
            std::string     __serverName;
            std::string     __host;
            __SIZE_TYPE__   __locationCount;
            LocationClass * __location;
            
        public:
            ServerConfig() : __port(8080), __serverName(""), __host(""), __locationCount(0) {}
            ServerConfig( ServerConfig const &x ) { *this = x; }
            ServerConfig& operator= ( const ServerConfig& rhs ) {
                if (this != &rhs) {
                    this->__port = rhs.__port;
                    this->__serverName = rhs.__serverName;
                    this->__host = rhs.__host;
                    this->__locationCount = rhs.__locationCount;
                    // this->__location = rhs.__location;
                }
                return *this;
            }
            ~ServerConfig() {}

            // This method should become private later
            __SIZE_TYPE__   getLocationCount() const { return this->__locationCount; }
            __SIZE_TYPE__   getPort() const { return this->__port; }
            std::string     getServerName() const { return this->__serverName; }
            std::string     getHost() const { return this->__host; }

            
    };
}