/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-fadi <sel-fadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/03/07 13:05:05 by sel-fadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include "../config/ConfigClass.hpp"
#include "../request/Request.hpp"
#include "../../CGI/cgi.hpp"
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
		void		_getResrc( std::string absPath );
		void		_postResrc( std::string absPath );
		void		_deleteResrc( std::string absPath );
		void		_autoindexModule( void );
		void		_extractData( void );
		
	public:
		void		errorMsg( std::string type );
		void		serveRequest( void );
		void		setData(ServerConfigClass config, Request req);
		void		setBytesSent(size_t bytesSent);
		void		offsetCursor(off_t offset);
		std::string	getBodyContent( void );
		void		headersSent();

	public:
		std::string	getHeaders( void ) const;
		std::string	getBody( void ) const;
		ServerConfigClass	getConfig( void) const;
		Request		getRequest( void ) const;
		std::string	getFileName( void ) const;
		int			getPos( void ) const;
		bool		getHeaderStatus( void ) const;
		bool		getStatus( void ) const;
		bool		isError( void ) const;
		size_t		getBodySize( void ) const;
		bool		bodyEof( void ) const;
		
	
	private:
		std::string			_headers;
		std::string			_body;
		int					_bodyFd;
		size_t				_bodySize;
		size_t				_totalSent;
		bool				_headersSent;
		bool				_error;
		ServerConfigClass	_config;
		Request				_req;
		std::string			_fileName;
		int					_pos; // should default to -1 if no location for said path
		
		cgi					_cgi;
		
};

#endif // WEBSERV_RESPONSE_HPP
