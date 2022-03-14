/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 13:32:54 by sel-fadi          #+#    #+#             */
/*   Updated: 2022/03/14 15:37:07 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <map>
#include <cstdlib>
#include <vector>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <istream>
#include <sys/types.h>
#include <signal.h>
#include <string>
#include <algorithm>
#include "../src/request/Request.hpp"

class cgi {
	public:
		Request _request;
		std::string arg;
		std::string scriptType;

	public:
		cgi();
		~cgi();
		cgi( cgi const &obj );
		cgi& operator=( cgi const &rhs );

		void processing_cgi( Request request );

	private:		
		void _exec_script( std::string filename );
		void _exec_scriptGET(int fd);
		std::string _getDate();
		void _setRequest(Request request);
		std::string	_generateTmp( int fd );
		void _setEnv();
		void _parseOutput( int fd );

	private:
		std::string	_status;
		std::string	_location;
		size_t		_contentLength;
		std::string	_output;
		std::string	_tmp;
		std::string	_tmpOutputFileName;

	public:
		std::string	getContent( void ) const;

};
