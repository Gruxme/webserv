/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:45:02 by aabounak          #+#    #+#             */
/*   Updated: 2022/02/22 14:22:39 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ConfigFile.hpp"

/* ----- Constructors & Destructor respectively ----- */
ConfigFile::ConfigFile() : __configFile("./conf.d/default.conf"), __serverCount(0), __serverConf() {}
ConfigFile::ConfigFile( std::string const & configFile ) { this->__configFile = configFile; }
ConfigFile::ConfigFile( ConfigFile const &rhs ) { *this = rhs; }
ConfigFile & ConfigFile::operator =( ConfigFile const & rhs) { this->__configFile = rhs.__configFile; return *this; }
ConfigFile::~ConfigFile() {}

/* ----- Getters ---- */
std::string 	ConfigFile::getConfigFile( void ) const { return this->__configFile; }
size_t			ConfigFile::getServerCount( void ) const { return this->__serverCount; }
ServerConfig	*ConfigFile::getServerConfig( void ) const { return __serverConf; }

/* ----- Setters ---- */
void    ConfigFile::__setServers( void ) {
    std::ifstream   file(this->__configFile);
    std::string     buffer;
    size_t   n = 0;
    while (getline(file, buffer)) {
        if (buffer.find("server {") != std::string::npos)
            n++;
    }
    this->__serverCount = n;
    this->__serverConf = new ServerConfig[this->__serverCount];
}

void    ConfigFile::__setLocations( void ) {
    std::ifstream   file(this->__configFile);
    std::string     buffer;
    size_t   n_serv = 0;
    size_t   n_loc = 0;
    while (getline(file, buffer)) {
        if (buffer.find("server {") != std::string::npos) {
            while (getline(file, buffer)) {
                if (buffer.find("}") != std::string::npos) {
                    this->__serverConf[n_serv].__locationCount = n_loc;
                    this->__serverConf[n_serv].__location = new LocationClass[n_loc];
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

/* ----- Main Parser ----- */
/* -- THIS PARSER SHOULD THROW EXCEPTIONS -- */
void    ConfigFile::parseConfigFile( void ) {
    std::ifstream	file(this->__configFile);
    std::string		buffer;
    size_t			n_serv = 0;
    this->__setServers();
    this->__setLocations();
    while (getline(file, buffer)) {
        if (buffer.find("server {") != std::string::npos) {
            size_t n_loc = 0;
            while (getline(file, buffer)) {
                if (buffer.find("}") != std::string::npos)
                    break ;
                if (buffer.find("#") != std::string::npos) 
                    continue ;
                else if (buffer.find("listen = ") != std::string::npos)
                    this->__serverConf[n_serv].__port = std::stoi(buffer.substr(buffer.find("listen = ") + strlen("listen = ")));
                else if (buffer.find("server_name = ") != std::string::npos)
                    this->__serverConf[n_serv].__serverName = buffer.substr(buffer.find("server_name = ") + strlen("server_name = "));
                else if (buffer.find("root = ") != std::string::npos)
                    this->__serverConf[n_serv].__root = buffer.substr(buffer.find("root = ") + strlen("root = "));
                else if (buffer.find("body_size_limit = ") != std::string::npos)
                    this->__serverConf[n_serv].__bodySizeLimit = std::stoi(buffer.substr(buffer.find("body_size_limit = ") + strlen("body_size_limit = ")));
                else if (buffer.find("access_log = ") != std::string::npos)
                    this->__serverConf[n_serv].__accessLog= buffer.substr(buffer.find("access_log = ") + strlen("access_log = "));
                else if (buffer.find("error_page = ") != std::string::npos)
                    this->__serverConf[n_serv].__errorPage= buffer.substr(buffer.find("error_page = ") + strlen("error_page = "));
                else if (buffer.find("autoindex = on") != std::string::npos)
                    this->__serverConf[n_serv].__autoindex = __AUTOINDEX_ON__;
                else if (buffer.find("location = [") != std::string::npos) {
                    while (getline(file, buffer)) {
                        if (buffer.find("]") != std::string::npos)
                            break ;
                        this->__serverConf[n_serv].__location[n_loc].parseLocation(buffer);
                    }
                    n_loc++;
                }
            }
            n_serv++;
        }
    }
	/* -- THIS PARSER SHOULD THROW EXCEPTIONS -- */
    /* Write final check Method */
}
