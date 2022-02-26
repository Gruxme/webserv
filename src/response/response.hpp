/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/02/24 18:08:06 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include "../config/ConfigClass.hpp"
#include "../request/Request.hpp"
#include <sys/stat.h>
#include <ctime>
#include <fcntl.h>
#include <poll.h>


class response {
	public:
		response();
		response( const response &x );
		~response();
		response&	operator=( const response &x );

	private:
		std::string	_errorMsg( std::string type, std::string statusCode );
		void		_getResrc( std::string data );
		void		_postResrc( std::string data );
		void		_deleteResrc( std::string data );
		void		_autoindexModule( void );

		void		_extractData( void );
		
	public:
		void		serveRequest( void );
		void		setData(ServerConfigClass config, Request req);
		bool		connStatus( void );
	private:
		std::string			_headers;
		std::fstream		_body;
		ServerConfigClass	_config;
		Request				_req;

	private:
		std::string	_scriptName;
		std::string	_path;
		int			_pos;
		bool		_status;
		
};

#endif // WEBSERV_RESPONSE_HPP
