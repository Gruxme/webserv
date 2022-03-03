/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationClass.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:45:04 by aabounak          #+#    #+#             */
/*   Updated: 2022/03/03 14:36:41 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "LocationClass.hpp"

/* ----- Constructors & Destructor respectively ----- */
LocationClass::LocationClass() : _path(), _root(), _method(), _redirect(), _cgiExt(), _autoindex(_AUTOINDEX_OFF_) {}
LocationClass::~LocationClass() {}
LocationClass::LocationClass( LocationClass const &x ) { *this = x; }
LocationClass & LocationClass::operator=( LocationClass const &rhs ) {
    if (this != &rhs) {
        this->_path = rhs._path;
        this->_root = rhs._root;
        this->_method = rhs._method;
        this->_redirect = rhs._redirect;
        this->_cgiExt = rhs._cgiExt;
        this->_autoindex = rhs._autoindex;
    }
    return *this;
}

/* ----- Location Parser ----- */
void    LocationClass::parseLocation( std::string buffer ) {
    if (!buffer.empty()) {
        switch (buffer[0]) {
            case 'a':
                if (std::strncmp("autoindex = ", buffer.c_str(), 12) == 0) {
                    if (std::strncmp("autoindex = on", buffer.c_str(), 14) == 0) this->_autoindex = _AUTOINDEX_ON_;
                    else if (std::strncmp("autoindex = off", buffer.c_str(), 15) == 0) this->_autoindex = _AUTOINDEX_OFF_;
                    break ;
                }
                throw parseErr("SyntaxError || Loc 1");
            case 'c':
                if (std::strncmp("cgi_ext = ", buffer.c_str(), 10) == 0) {
                    this->_cgiExt = buffer.substr(buffer.find("cgi_ext = ") + strlen("cgi_ext = "));
                    break ;
                }
                throw parseErr("SyntaxError || Loc 2");
            case 'm':
                if (std::strncmp("method = ", buffer.c_str(), 9) == 0) {
                    this->_method = buffer.substr(buffer.find("method = ") + strlen("method = "));
                    break ;
                }
                throw parseErr("SyntaxError || Loc 3");
            case 'p':
                if (std::strncmp("path = ", buffer.c_str(), 7) == 0) {
                    this->_path = buffer.substr(buffer.find("path = ") + strlen("path = "));
                    break ;
                }
                throw parseErr("SyntaxError || Loc 4");
            case 'r':
                if (std::strncmp("root = ", buffer.c_str(), 7) == 0) {
                    this->_root = buffer.substr(buffer.find("root = ") + strlen("root = "));
                    if (this->_root[this->_root.size() - 1 != '/']) this->_root += "/";
                    break ;
                }
                else if (std::strncmp("redirect = ", buffer.c_str(), 11) == 0) {
                    this->_redirect = buffer.substr(buffer.find("redirect = ") + strlen("redirect = "));
                    break ;
                }
                throw parseErr("SyntaxError || Loc 5");
            default:
                if (buffer.empty()) break ;
                throw parseErr("SyntaxError || LOCATION SIDE");
        }
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
std::string LocationClass::getMethod( void ) const { return this->_method; }
std::string LocationClass::getRedirect( void ) const { return this->_redirect; }
std::string LocationClass::getCgiExt( void ) const { return this->_cgiExt; }
bool        LocationClass::getAutoIndex( void ) const { return this->_autoindex; }
