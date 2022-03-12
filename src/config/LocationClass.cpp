/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationClass.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:45:04 by aabounak          #+#    #+#             */
/*   Updated: 2022/03/12 20:50:53 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "LocationClass.hpp"

/* ----- Constructors & Destructor respectively ----- */
LocationClass::LocationClass() :
    _path(""),
    _root(""),
    _methods(),
    _redirect(""),
    _upload(""),
    _cgi(),
    _autoindex(_AUTOINDEX_OFF_) {}

LocationClass::~LocationClass() {}
LocationClass::LocationClass( LocationClass const &x ) { *this = x; }
LocationClass & LocationClass::operator=( LocationClass const &rhs ) {
    if (this != &rhs) {
        this->_path = rhs._path;
        this->_root = rhs._root;
        this->_methods = rhs._methods;
        this->_redirect = rhs._redirect;
        this->_upload = rhs._upload;
        this->_cgi = rhs._cgi;
        this->_autoindex = rhs._autoindex;
    }
    return *this;
}

/* ----- Location Parser ----- */
void    LocationClass::parseLocation( std::string buffer ) {
    bool processedAlready = false;
    if (!buffer.empty()) {
        switch (buffer[0]) {
            case 'a':
                if (processedAlready == false && std::strncmp("autoindex = ", buffer.c_str(), 12) == 0) {
                    if (std::strncmp("autoindex = on", buffer.c_str(), 14) == 0) this->_autoindex = _AUTOINDEX_ON_;
                    else if (std::strncmp("autoindex = off", buffer.c_str(), 15) == 0) this->_autoindex = _AUTOINDEX_OFF_;
                    break ;
                }
                throw parseErr("SyntaxError || Loc 1");
            case 'c':
                if (this->_cgi.empty() && std::strncmp("cgi = ", buffer.c_str(), 6) == 0) {
                    std::string x = buffer.substr(buffer.find("cgi = ") + strlen("cgi = "));
                    std::vector<std::string> tmp = _split(x, ' ');
                    for (size_t i = 0; i < tmp.size(); i++) {
                        tmp[i] = _trim(tmp[i], " ");
                        this->_cgi.push_back(tmp[i]);
                    }
                    if (_cgi.size() > 2)
                        throw parseErr("SyntaxError || CGI");
                    break ;
                }
                throw parseErr("SyntaxError || Loc 2");
            case 'm':
                if (this->_methods.empty() && std::strncmp("method = ", buffer.c_str(), 9) == 0) {
                    std::string x = buffer.substr(buffer.find("method = ") + strlen("method = "));
                    std::vector<std::string> tmp = _split(x, ' ');
                    for (size_t i = 0; i < tmp.size(); i++) {
                        tmp[i] = _trim(tmp[i], " ");
                        this->_methods.push_back(tmp[i]);
                    }
                    if (_methods.size() > 3)
                        throw parseErr("SyntaxError || METHODS");
                    break ;
                }
                throw parseErr("SyntaxError || Loc 3");
            case 'p':
                if (this->_path.empty() && std::strncmp("path = ", buffer.c_str(), 7) == 0) {
                    this->_path = buffer.substr(buffer.find("path = ") + strlen("path = "));
                    if (this->_path[this->_path.size() - 1] != '/')
                        this->_path += "/";
                    break ;
                }
                throw parseErr("SyntaxError || Loc 4");
            case 'r':
                if (this->_root.empty() && std::strncmp("root = ", buffer.c_str(), 7) == 0) {
                    this->_root = buffer.substr(buffer.find("root = ") + strlen("root = "));
                    if (this->_root[this->_root.size() - 1] == '/')
                        this->_root.resize(this->_root.size() - 1);
                    break ;
                }
                else if (this->_redirect.empty() && std::strncmp("redirect = ", buffer.c_str(), 11) == 0) {
                    this->_redirect = buffer.substr(buffer.find("redirect = ") + strlen("redirect = "));
                    break ;
                }
                throw parseErr("SyntaxError || Loc 5");
            case 'u':
                if (this->_upload.empty() && std::strncmp("upload = ", buffer.c_str(), 0) == 0) {
                    this->_upload = buffer.substr(buffer.find("upload = ") + strlen("upload = "));
                    if (this->_upload[this->_upload.size() - 1] == '/')
                        this->_upload.resize(this->_upload.size() - 1);
                    break ;
                }
                throw parseErr("SyntaxError || Loc 6");
            default:
                if (buffer.empty()) break ;
                throw parseErr("SyntaxError || LOCATION SIDE");
        }
    }
    return ;
}

/* ----- Getters----- */
std::string LocationClass::getPath( void ) const { return this->_path; }
std::string LocationClass::getRoot( void ) const { return this->_root; }
std::vector<std::string> LocationClass::getMethods( void ) const { return this->_methods; }
std::string LocationClass::getRedirect( void ) const { return this->_redirect; }
std::string LocationClass::getUpload( void) const { return this->_upload; }
std::vector<std::string> LocationClass::getCgi( void ) const { return this->_cgi; }
bool        LocationClass::getAutoIndex( void ) const { return this->_autoindex; }
