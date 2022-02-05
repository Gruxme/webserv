/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationClass.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 01:50:10 by aabounak          #+#    #+#             */
/*   Updated: 2022/02/05 18:24:52 by aabounak         ###   ########.fr       */
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
            std::string __cgi_ext;
            
        public:
            /* ----- Constructors & Destructor respectively ----- */
            LocationClass() : __path(""), __root(""), __cgi_ext("") {}
            ~LocationClass() {}

            /* ----- PRIVATE ----- */
            /* ----- Location Parser ----- */
            void    parseLocation( std::string buffer ) {
                if (buffer.find("path = ") != std::string::npos)
                    this->__path = buffer.substr(buffer.find("path = ") + strlen("path = "));
                else if (buffer.find("root = ") != std::string::npos)
                    this->__root = buffer.substr(buffer.find("root = ") + strlen("root = "));
                else if (buffer.find("cgi_ext") != std::string::npos) {
                    this->__cgi_ext = buffer.substr(buffer.find("cgi_ext = ") + strlen("cgi_ext = "));
                }
            }

            std::vector<std::string> split( std::string str, char separator ) {
                std::vector<std::string>  myvec;
                __SIZE_TYPE__ currentIndex = 0, i = 0, startIndex = 0, endIndex = 0;
                while (i <= str.length()) {
                    if (str[i] == separator || i == str.length()) {
                        endIndex = i;
                        std::string subString = "";
                        subString.append(str, startIndex, endIndex - startIndex);
                        myvec.push_back(subString);
                        currentIndex++;
                        startIndex = endIndex + 1;
                    }
                    i++;
                }
                return myvec;
            }

            /* ----- Getters----- */
            std::string getPath( void ) const { return this->__path; }
            std::string getRoot( void ) const { return this->__root; }
            std::string getCgiExt( void ) const { return this->__cgi_ext; }
    };    
}
