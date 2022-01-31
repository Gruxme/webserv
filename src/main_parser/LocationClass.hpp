/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationClass.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 01:50:10 by aabounak          #+#    #+#             */
/*   Updated: 2022/01/31 15:31:57 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "ServerConfig.hpp"

namespace ft {
    class LocationClass {
        /* ----- PRIVATE ----- */
        public:
            std::string __path;
            std::string __root;
            
        public:
            /* ----- Constructors & Destructor respectively ----- */
            LocationClass() : __path(""), __root("") {}
            ~LocationClass() {}

            /* ----- PRIVATE ----- */
            /* ----- Location Parser ----- */
            void    parseLocation( std::string buffer ) {
                if (buffer.find("path = ") != std::string::npos)
                    this->__path = buffer.substr(buffer.find("path = ") + strlen("path = "));
                else if (buffer.find("root = ") != std::string::npos)
                    this->__root = buffer.substr(buffer.find("root = ") + strlen("root = "));
            }

            /* ----- Getters----- */
            std::string getPath( void ) const { return this->__path; }
            std::string getRoot( void ) const { return this->__root; }
    };    
}
