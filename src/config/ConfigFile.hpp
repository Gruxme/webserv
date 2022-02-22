/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 11:14:54 by aabounak          #+#    #+#             */
/*   Updated: 2022/01/20 18:12:59 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <string>
# include <iostream>
# include <fstream>
# include <algorithm>
# include "ServerConfig.hpp"

class ConfigFile {
	/* ----- PRIVATE ----- */
	private:
		std::string		__configFile;
		size_t			__serverCount;
		ServerConfig *	__serverConf;

	private:
		/* ----- SETTERS ----- */
		void	__setServers( void );
		void	__setLocations( void );

	public:
		/* ----- Constructors & Destructor respectively ----- */
		ConfigFile();
		ConfigFile( std::string const & configFile );
		ConfigFile( ConfigFile const &rhs );
		ConfigFile & operator =( ConfigFile const & rhs);
		~ConfigFile();

		/* ----- Getters ---- */
		std::string			getConfigFile( void ) const;
		size_t				getServerCount( void ) const;
		ServerConfig		getServerConfig( void ) const;

		/* ----- Main Parser ----- */
		/* -- THIS PARSER SHOULD THROW EXCEPTIONS -- */
		void	parseConfigFile( void );

		/* ----- Exceptions ----- */
		class ParsingError : public std::exception {
		public:
			virtual const char * what() const throw() {
				return ("ParsingError");
			}
	};
};
