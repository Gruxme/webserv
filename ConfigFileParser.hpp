/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFileParser.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 11:14:54 by aabounak          #+#    #+#             */
/*   Updated: 2022/01/20 18:12:59 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "header.hpp"
# include "ServerConfig.hpp"

namespace ft {
    class ConfigFileParser {
        /* These will become private */
        public:
            std::string     __configFile;
            __SIZE_TYPE__   __serverCount;
            // std::vector<ServerConfig>   __serverConf;
            ServerConfig *  __serverConf;

        public:
            ConfigFileParser() : __configFile("./config.default"), __serverCount(0), __serverConf() {}
            ConfigFileParser( std::string const & configFile ) { this->__configFile = configFile; }
            ConfigFileParser( ConfigFileParser const &rhs ) { *this = rhs; }
            ConfigFileParser & operator =( ConfigFileParser const & rhs) { this->__configFile = rhs.__configFile; return *this; }
            ~ConfigFileParser() {}

            std::string     getConfigFile() const { return this->__configFile; }

            // This method should become private later
            __SIZE_TYPE__   __getServerCount() const {
                std::ifstream   file(this->__configFile);
                std::string     buffer;
                __SIZE_TYPE__   count = 0;
                while (getline(file, buffer)) {
                    if (buffer.find("server {") != std::string::npos)
                        count++;
                }
                return count;
            }
            void            __setLocations( void ) {
                std::ifstream   file(this->__configFile);
                std::string     buffer;
                __SIZE_TYPE__   serverCounter = 0;
                __SIZE_TYPE__   locationCounter = 0;
                while (getline(file, buffer)) {
                    if (buffer.find("server {") != std::string::npos) {
                        while (getline(file, buffer)) {
                            if (buffer.find("}") != std::string::npos) {
                                this->__serverConf[serverCounter].__locationCount = locationCounter;
                                this->__serverConf[serverCounter].__location = new LocationClass[locationCounter];
                                serverCounter++;
                                locationCounter = 0;
                                break ;
                            }
                            else if (buffer.find("location = [") != std::string::npos)
                                locationCounter++;
                        }
                    }
                }
            }
            void    parseConfigFile( void ) {
                std::ifstream   file(this->__configFile);
                std::string     buffer;
                __SIZE_TYPE__   serverCounter = 0;
                this->__serverConf = new ServerConfig[this->__serverCount];
                this->__serverCount = this->__getServerCount();
                this->__setLocations();
                while (getline(file, buffer)) {
                    if (buffer.find("server {") != std::string::npos) {
                        __SIZE_TYPE__ locationCounter = 0;
                        while (getline(file, buffer)) {
                            if (buffer.find("}") != std::string::npos)
                                break ;
                            else if (buffer.find("listen = ") != std::string::npos)
                                this->__serverConf[serverCounter].__port = std::stoi(buffer.substr(buffer.find("listen = ") + strlen("listen = ")));
                            else if (buffer.find("server_name = ") != std::string::npos)
                                this->__serverConf[serverCounter].__serverName = buffer.substr(buffer.find("server_name = ") + strlen("server_name = "));
                            else if (buffer.find("host = ") != std::string::npos)
                                this->__serverConf[serverCounter].__host = buffer.substr(buffer.find("host = ") + strlen("host = "));
                            else if (buffer.find("location = [") != std::string::npos) {
                                while (getline(file, buffer)) {
                                    if (buffer.find("]") != std::string::npos)
                                        break ;
                                    this->__serverConf[serverCounter].__location[locationCounter].parseLocation(buffer);
                                }
                                locationCounter++;
                            }
                        }
                        serverCounter++;
                    }
                }
            }
            
            class ParsingError : public std::exception {
			public:
				virtual const char * what() const throw() {
					return ("ParsingError");
				}
		};
    };
}