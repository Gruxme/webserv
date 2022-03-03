/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigClass.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:45:02 by aabounak          #+#    #+#             */
/*   Updated: 2022/03/03 11:08:24 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ConfigClass.hpp"

/* ----- Constructors & Destructor respectively ----- */
ConfigClass::ConfigClass() : _configFile("./conf.d/default.conf"), _serverCount(0), _serverConf(0) {}
ConfigClass::ConfigClass( std::string const & configFile ) { this->_configFile = configFile; }
ConfigClass::ConfigClass( ConfigClass const &rhs ) { *this = rhs; }
ConfigClass & ConfigClass::operator =( ConfigClass const & rhs) {
    if (this != &rhs) {
        this->_configFile = rhs._configFile; 
        this->_serverCount = rhs._serverCount;
        /* -- {DEEP COPY} _serverConf */
        this->_serverConf = rhs._serverConf;
    }
    return *this;
}
ConfigClass::~ConfigClass() {}

/* ----- Getters ---- */
std::string 	ConfigClass::getConfigFile( void ) const { return this->_configFile; }
size_t			ConfigClass::getServerCount( void ) const { return this->_serverCount; }
std::vector<ServerConfigClass> ConfigClass::getServerConfigClass( void ) const { return _serverConf; }

/* ----- Setters ---- */
void    ConfigClass::_allocateServers( void ) {
    std::ifstream   file(this->_configFile);
    std::string     buffer;
    size_t   n = 0;
    while (getline(file, buffer)) {
        if (buffer.find("server {") != std::string::npos)
            n++;
    }
    this->_serverCount = n;
    for (size_t i = 0; i < this->_serverCount; i++)
        this->_serverConf.push_back(ServerConfigClass());
}

void    ConfigClass::_allocateLocations( void ) {
    std::ifstream   file(this->_configFile);
    std::string     buffer;
    size_t   n_serv = 0;
    size_t   n_loc = 0;
    while (getline(file, buffer)) {
        if (buffer.find("server {") != std::string::npos) {
            while (getline(file, buffer)) {
                if (buffer.find("}") != std::string::npos) {
                    this->_serverConf[n_serv]._locationCount = n_loc;
                    for (size_t i = 0; i < n_loc; i++)
                        this->_serverConf[n_serv]._location.push_back(LocationClass());
                    n_serv++;
                    n_loc = 0;
                    break ;
                }
                else if (buffer.find("location = [") != std::string::npos)
                    n_loc++;
            }
        }
    }
}

void    ConfigClass::_checkConfigValidity( void ) {
    
    return ;
}

/* ----- Main Parser ----- */
/* -- THIS PARSER SHOULD THROW EXCEPTIONS -- */
void    ConfigClass::parseConfigFile( void ) {
    std::ifstream	file(this->_configFile);
    std::string		buffer;
    size_t			n_serv = 0; 
    this->_allocateServers();
    this->_allocateLocations();
    while (getline(file, buffer)) {
        if (buffer.find("server {") != std::string::npos) {
            size_t n_loc = 0;
            while (getline(file, buffer)) {
                if (buffer.find("}") != std::string::npos)
                    break ;
                if (buffer.find("#") != std::string::npos) 
                    continue ;
                else if (buffer.find("listen = ") != std::string::npos)
                    this->_serverConf[n_serv]._port = std::stoi(buffer.substr(buffer.find("listen = ") + strlen("listen = ")));
                else if (buffer.find("server_name = ") != std::string::npos)
                    this->_serverConf[n_serv]._serverName = buffer.substr(buffer.find("server_name = ") + strlen("server_name = "));
                else if (buffer.find("root = ") != std::string::npos) {
                    this->_serverConf[n_serv]._root = buffer.substr(buffer.find("root = ") + strlen("root = "));
                    if (this->_serverConf[n_serv]._root[this->_serverConf[n_serv]._root.size() - 1 != '/']) this->_serverConf[n_serv]._root += "/";
                }
                else if (buffer.find("body_size_limit = ") != std::string::npos)
                    this->_serverConf[n_serv]._bodySizeLimit = std::stoi(buffer.substr(buffer.find("body_size_limit = ") + strlen("body_size_limit = ")));
                else if (buffer.find("access_log = ") != std::string::npos)
                    this->_serverConf[n_serv]._accessLog= buffer.substr(buffer.find("access_log = ") + strlen("access_log = "));
                else if (buffer.find("error_page = ") != std::string::npos)
                    this->_serverConf[n_serv]._errorPage= buffer.substr(buffer.find("error_page = ") + strlen("error_page = "));
                else if (buffer.find("autoindex = on") != std::string::npos)
                    this->_serverConf[n_serv]._autoindex = _AUTOINDEX_ON_;
                else if (buffer.find("location = [") != std::string::npos) {
                    while (getline(file, buffer)) {
                        if (buffer.find("]") != std::string::npos)
                            break ;
                        this->_serverConf[n_serv]._location[n_loc].parseLocation(buffer);
                    }
                    n_loc++;
                }
                else { throw parseErr("Parsing Error\nRemember to read comments in the default.conf file for usage guide\n"); }
            }
            n_serv++;
        }
    }
	/* -- THIS PARSER SHOULD THROW EXCEPTIONS -- */
    /* Write final check Method */
}
