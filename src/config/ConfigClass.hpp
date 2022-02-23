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
# include <cstring>
# include <iostream>
# include <fstream>
# include <algorithm>
# include "ServerConfigClass.hpp"

class ConfigClass {

	private:
		std::string		_configFile;
		size_t			_serverCount;
		ServerConfigClass *	_serverConf;

	private:
		void	_allocateServers( void );
		void	_allocateLocations( void );

	public:
		ConfigClass();
		ConfigClass( std::string const & ConfigClass );
		ConfigClass( ConfigClass const &rhs );
		ConfigClass & operator =( ConfigClass const & rhs);
		~ConfigClass();

		std::string			getConfigFile( void ) const;
		size_t				getServerCount( void ) const;
		ServerConfigClass	*getServerConfigClass( void ) const;

		void	parseConfigFile( void );

		class ParsingError : public std::exception {
		public:
			virtual const char * what() const throw() {
				return ("ParsingError");
			}
	};
};
