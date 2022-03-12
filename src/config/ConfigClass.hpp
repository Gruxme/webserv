/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 11:14:54 by aabounak          #+#    #+#             */
/*   Updated: 2022/01/20 18:12:59 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <string>
# include <vector>
# include <cstring>
# include <iostream>
# include <sstream>
# include <fstream>
# include <algorithm>
# include "ServerConfigClass.hpp"

class ConfigClass {

	private:
		std::string		_configFile;
		size_t			_serverCount;
		std::vector<ServerConfigClass>	_serverConf;

	private:
		void	_allocateServers( void );
		void	_allocateLocations( void );

	public:
		ConfigClass();
		ConfigClass( std::string const & ConfigClass );
		ConfigClass( ConfigClass const &rhs );
		ConfigClass & operator =( ConfigClass const & rhs);
		~ConfigClass();

		std::string		getConfigFile( void ) const;
		size_t			getServerCount( void ) const;
		std::vector<ServerConfigClass>	getServerConfigClass( void ) const;

		void	parseConfigFile( void );

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

		class parseErr : public std::exception {
        public:
            explicit parseErr( const std::string &errStr ) throw() : _errStr(errStr) {}
            ~parseErr() throw() {}
            virtual const char * what() const throw() {
                return (_errStr.c_str());
            }
        private:
            std::string _errStr;
        };
};
