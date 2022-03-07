/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigClass.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:45:02 by aabounak          #+#    #+#             */
/*   Updated: 2022/03/07 19:39:13 by aabounak         ###   ########.fr       */
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
        this->_serverConf = rhs._serverConf;
    }
    return *this;
}
ConfigClass::~ConfigClass() {}

/* ----- Getters ---- */
std::string 	ConfigClass::getConfigFile( void ) const { return this->_configFile; }
size_t			ConfigClass::getServerCount( void ) const { return this->_serverCount; }
std::vector<ServerConfigClass> ConfigClass::getServerConfigClass( void ) const { return _serverConf; }

std::string _ltrim(const std::string &s, const std::string set ) {
    size_t start = s.find_first_not_of(set);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string _rtrim(const std::string &s, const std::string set ) {
    size_t end = s.find_last_not_of(set);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string _trim(const std::string &s, const std::string set ) {
    return _rtrim(_ltrim(s, set), set);
}

/* ----- Setters ---- */
void    ConfigClass::_allocateServers( void ) {
    std::ifstream   file(this->_configFile);
    std::string     buffer;
    size_t   n = 0;
    while (getline(file, buffer)) {
        buffer = _trim(buffer, " ");
        if (buffer == "server {")
            n++;
    }
    n > 0 ? this->_serverCount = n : throw parseErr("Invalid Server");
    for (size_t i = 0; i < this->_serverCount; i++)
        this->_serverConf.push_back(ServerConfigClass());
}

void    ConfigClass::_allocateLocations( void ) {
    std::ifstream   file(this->_configFile);
    std::string     buffer;
    size_t   n_serv = 0;
    size_t   n_loc = 0;
    while (getline(file, buffer)) {
        buffer = _trim(buffer, " ");
        if (buffer == "server {") {
            while (getline(file, buffer)) {
                buffer = _trim(buffer, " ");
                if (buffer.find("}") != std::string::npos && buffer != "}") throw parseErr("SyntaxError || F U");
                if (buffer == "}") {
                    
                    n_loc > 0 ? this->_serverConf[n_serv]._locationCount = n_loc : throw parseErr("Invalid Locations");
                    for (size_t i = 0; i < n_loc; i++) {
                        this->_serverConf[n_serv]._location.push_back(LocationClass());
                    }
                    n_serv++;
                    n_loc = 0;
                    break ;
                }
                else if (buffer == "location = [")
                    n_loc++;
            }
        }
    }
}

/* ----- Main Parser ----- */

void    ConfigClass::parseConfigFile( void ) {
    std::ifstream	file(this->_configFile);
    std::string		buffer;
    size_t			n_serv = 0; 
    this->_allocateServers();
    this->_allocateLocations();
    while (getline(file, buffer)) {
        buffer = _trim(buffer, " ");
        if (buffer != "server {") {
            if ((buffer[0] == '#' && buffer.find_first_of("#") == 0) || buffer.empty()) continue ;
            else throw parseErr("SyntaxError || WEIRD AF");
        }
        else {
            size_t n_loc = 0;
            while (getline(file, buffer)) {
                buffer = _trim(buffer, " ");
                if (buffer.find("#") != std::string::npos) {
                    std::cout << buffer << std::endl;
                }
                if (buffer == "}") break ;
                switch (buffer[0]) {
                    case 'a':
                        if (std::strncmp("access_log = ", buffer.c_str(), 13) == 0) {
                            this->_serverConf[n_serv]._accessLog = buffer.substr(buffer.find("access_log = ") + strlen("access_log = "));
                            break ;
                        }
                        throw parseErr("SyntaxError || 1");
                    case 'b':
                        if (std::strncmp("body_size_limit = ", buffer.c_str(), 18) == 0) {
                            this->_serverConf[n_serv]._bodySizeLimit = std::stoi(buffer.substr(buffer.find("body_size_limit = ") + strlen("body_size_limit = ")));
                            break ;
                        }
                        throw parseErr("SyntaxError || 2");
                    case 'e':
                        if (std::strncmp("error_page = ", buffer.c_str(), 13) == 0) {
                            this->_serverConf[n_serv]._errorPage = buffer.substr(buffer.find("error_page = ") + strlen("error_page = "));
                            break ;
                        }
                        throw parseErr("SyntaxError || 3");
                    case 'l':
                        if (std::strncmp("listen = ", buffer.c_str(), 9) == 0) {
                            this->_serverConf[n_serv]._port = std::stoi(buffer.substr(buffer.find("listen = ") + strlen("listen = ")));
                            break ;
                        }
                        else if (std::strncmp("location = [", buffer.c_str(), 12) == 0) {
                            while (getline(file, buffer)) {
                                buffer = _trim(buffer, " ");
                                if (buffer == "]") break ;
                                this->_serverConf[n_serv]._location[n_loc].parseLocation(buffer);
                            }
                            n_loc++;
                            break ;
                        }
                        throw parseErr("SyntaxError || 4");       
                    case 's':
                        if (std::strncmp("server_name = ", buffer.c_str(), 14) == 0) {
                            this->_serverConf[n_serv]._serverName = buffer.substr(buffer.find("server_name = ") + strlen("server_name = "));
                            break ;
                        }
                        throw parseErr("SyntaxError || 6");
                    default:
                        if (buffer.empty()) break ;
                        throw parseErr("SyntaxError || SERVER SIDE");
                }
            }
            n_serv++;
        }
    }
}
