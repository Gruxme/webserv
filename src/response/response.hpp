/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/03/01 11:04:35 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include "../config/ConfigClass.hpp"
#include "../request/Request.hpp"
#include "mimeTypes.hpp"
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
		void		_errorMsg( std::string type, std::string statusCode );
		void		_getResrc( std::string data );
		void		_postResrc( std::string data );
		void		_deleteResrc( std::string data );
		void		_autoindexModule( void );

		void		_extractData( void );
		
	public:
		void		serveRequest( void );
		void		setData(ServerConfigClass config, Request req);
		void		setSendStatus(bool status, size_t _bytesSent);
		bool		getSendStatus( void );
		std::string	getBodyContent( void );
		bool		connStatus( void );

	public:
		std::string	getHeaders( void ) const;
		std::string	getBody( void ) const;
		/* 
			1/2 WRITE A COUPLE OF GETTERS FOR THE CONFIG / REQ PVT ATTRIBUTES 
		*/
		std::string	getFileName( void ) const;
		std::string	getPath( void ) const;
		int			getPos( void ) const;
		bool		getHeaderStatus( void ) const;
		bool		getStatus( void ) const;
		
	
	private:
		std::string			_headers;
		std::string			_body;
		int					_bodyFd;
		size_t				_bodySize;
		size_t				_bytesSent;
		bool				_sendStatus;
		ServerConfigClass	_config;
		Request				_req;

	private:
		std::string	_fileName;
		std::string	_path;
		int			_pos; // should default to -1 if no location for said path
		bool		_headersStatus;
		bool		_status;
		
};

#endif // WEBSERV_RESPONSE_HPP
