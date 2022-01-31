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
# include "ServerConfig.hpp"
namespace ft {
    class ConfigFileParser {
        /* ----- PRIVATE ----- */
        public:
            std::string     __configFile;
            __SIZE_TYPE__   __serverCount;
            ServerConfig *  __serverConf;

        public:
            /* ----- Constructors & Destructor respectively ----- */
            ConfigFileParser() : __configFile("./conf.d/default.conf"), __serverCount(0), __serverConf() {}
            ConfigFileParser( std::string const & configFile ) { this->__configFile = configFile; }
            ConfigFileParser( ConfigFileParser const &rhs ) { *this = rhs; }
            ConfigFileParser & operator =( ConfigFileParser const & rhs) { this->__configFile = rhs.__configFile; return *this; }
            ~ConfigFileParser() {}

            /* ----- Getters ---- */
            std::string     getConfigFile() const { return this->__configFile; }

            /* ----- PRIVATE ----- */
            void    __setServers() {
                std::ifstream   file(this->__configFile);
                std::string     buffer;
                __SIZE_TYPE__   n = 0;
                while (getline(file, buffer)) {
                    if (buffer.find("server {") != std::string::npos)
                        n++;
                }
                this->__serverCount = n;
                this->__serverConf = new ServerConfig[this->__serverCount];
            }

            /* ----- PRIVATE ----- */
            void    __setLocations( void ) {
                std::ifstream   file(this->__configFile);
                std::string     buffer;
                __SIZE_TYPE__   n_serv = 0;
                __SIZE_TYPE__   n_loc = 0;
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
            void    parseConfigFile( void ) {
                std::ifstream   file(this->__configFile);
                std::string     buffer;
                __SIZE_TYPE__   n_serv = 0;
                this->__setServers();
                this->__setLocations();
                while (getline(file, buffer)) {
                    if (buffer.find("server {") != std::string::npos) {
                        __SIZE_TYPE__ n_loc = 0;
                        while (getline(file, buffer)) {
                            if (buffer.find("}") != std::string::npos)
                                break ;
                            else if (buffer.find("listen = ") != std::string::npos)
                                this->__serverConf[n_serv].__port = std::stoi(buffer.substr(buffer.find("listen = ") + strlen("listen = ")));
                            else if (buffer.find("server_name = ") != std::string::npos)
                                this->__serverConf[n_serv].__serverName = buffer.substr(buffer.find("server_name = ") + strlen("server_name = "));
                            else if (buffer.find("host = ") != std::string::npos)
                                this->__serverConf[n_serv].__host = buffer.substr(buffer.find("host = ") + strlen("host = "));
                            else if (buffer.find("body_size_limit = ") != std::string::npos)
                                this->__serverConf[n_serv].__bodySizeLimit = std::stoi(buffer.substr(buffer.find("body_size_limit = ") + strlen("body_size_limit = ")));
                            else if (buffer.find("access_log = ") != std::string::npos)
                                this->__serverConf[n_serv].__accessLog= buffer.substr(buffer.find("access_log = ") + strlen("access_log = "));
                            else if (buffer.find("error_page = ") != std::string::npos)
                                this->__serverConf[n_serv].__errorPage= buffer.substr(buffer.find("error_page = ") + strlen("error_page = "));
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
                /* Write check Method */
            }

            /* ----- Exceptions ----- */
            class ParsingError : public std::exception {
			public:
				virtual const char * what() const throw() {
					return ("ParsingError");
				}
		};
    };
}
