/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationClass.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:45:04 by aabounak          #+#    #+#             */
/*   Updated: 2022/02/23 11:31:36 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "LocationClass.hpp"

/* ----- Constructors & Destructor respectively ----- */
LocationClass::LocationClass() : _path(""), _root(""), _cgiExt(""), _autoindex(_AUTOINDEX_OFF_) {}
LocationClass::~LocationClass() {}
LocationClass::LocationClass( LocationClass const &x ) { *this = x; }
LocationClass & LocationClass::operator=( LocationClass const &rhs ) {
    if (this != &rhs) {
        this->_path = rhs._path;
        this->_root = rhs._root;
        this->_cgiExt = rhs._cgiExt;
        this->_autoindex = rhs._autoindex;
    }
    return *this;
}

/* ----- Location Parser ----- */
void    LocationClass::parseLocation( std::string buffer ) {
    if (!buffer.empty()) {
        if (buffer.find("path = ") != std::string::npos)
            this->_path = buffer.substr(buffer.find("path = ") + strlen("path = "));
        else if (buffer.find("root = ") != std::string::npos)
            this->_root = buffer.substr(buffer.find("root = ") + strlen("root = "));
        else if (buffer.find("cgi_ext") != std::string::npos)
            this->_cgiExt = buffer.substr(buffer.find("cgi_ext = ") + strlen("cgi_ext = "));
        else if (buffer.find("autoindex = on") != std::string::npos)
            this->_autoindex = _AUTOINDEX_ON_;
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
std::string LocationClass::getPath( void ) const { return this->_path; }
std::string LocationClass::getRoot( void ) const { return this->_root; }
std::string LocationClass::getCgiExt( void ) const { return this->_cgiExt; }
bool        LocationClass::getAutoIndex( void ) const { return this->_autoindex; }
