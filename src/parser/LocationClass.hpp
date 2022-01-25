/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationClass.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 01:50:10 by aabounak          #+#    #+#             */
/*   Updated: 2022/01/25 04:59:43 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "ServerConfig.hpp"

namespace ft {
    class LocationClass {
        /* These will become private */
        public:
            std::string __path;
            std::string __root;
            std::string __access_log;
            
        public:
            LocationClass() : __path(""), __root(""), __access_log("") {}
            ~LocationClass() {}

            void    parseLocation( std::string buffer ) {
                if (buffer.find("path = ") != std::string::npos)
                    this->__path = buffer.substr(buffer.find("path = ") + strlen("path = "));
                else if (buffer.find("root = ") != std::string::npos)
                    this->__root = buffer.substr(buffer.find("root = ") + strlen("root = "));
                else if (buffer.find("access_log = ") != std::string::npos)
                    this->__access_log = buffer.substr(buffer.find("access_log = ") + strlen("access_log = "));
            }

            std::string getPath( void ) const { return this->__path; }
            std::string getRoot( void ) const { return this->__root; }
            std::string getAccessLog( void ) const { return this->__access_log; }
    };    
}
