/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationClass.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:45:04 by aabounak          #+#    #+#             */
/*   Updated: 2022/02/23 10:51:10 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "LocationClass.hpp"

/* ----- Constructors & Destructor respectively ----- */
LocationClass::LocationClass() : __path(""), __root(""), __cgiExt(""), __autoindex(__AUTOINDEX_OFF__) {}
LocationClass::~LocationClass() {}
LocationClass::LocationClass( LocationClass const &x ) { *this = x; }
LocationClass & LocationClass::operator=( LocationClass const &rhs ) {
    if (this != &rhs) {
        this->__path = rhs.__path;
        this->__root = rhs.__root;
        this->__cgiExt = rhs.__cgiExt;
        this->__autoindex = rhs.__autoindex;
    }
    return *this;
}

/* ----- Location Parser ----- */
void    LocationClass::parseLocation( std::string buffer ) {
    if (!buffer.empty()) {
        if (buffer.find("path = ") != std::string::npos)
            this->__path = buffer.substr(buffer.find("path = ") + strlen("path = "));
        else if (buffer.find("root = ") != std::string::npos)
            this->__root = buffer.substr(buffer.find("root = ") + strlen("root = "));
        else if (buffer.find("cgi_ext") != std::string::npos)
            this->__cgiExt = buffer.substr(buffer.find("cgi_ext = ") + strlen("cgi_ext = "));
        else if (buffer.find("autoindex = on") != std::string::npos)
            this->__autoindex = __AUTOINDEX_ON__;
    }
    return ;
}

/* -- PUBLIC METHODS */
std::vector<std::string> LocationClass::split( std::string str, char separator ) {
    std::vector<std::string>  myvec;
    size_t currentIndex = 0, i = 0, startIndex = 0, endIndex = 0;
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
std::string LocationClass::getPath( void ) const { return this->__path; }
std::string LocationClass::getRoot( void ) const { return this->__root; }
std::string LocationClass::getCgiExt( void ) const { return this->__cgiExt; }
bool        LocationClass::getAutoIndex( void ) const { return this->__autoindex; }
