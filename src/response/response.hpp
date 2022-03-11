  /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/03/07 15:30:40 by abiari           ###   ########.fr       */
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
#include <dirent.h>
#include <stdio.h>

class response {
	public:
		response();
		response( const response &x );
		~response();
		response&	operator=( const response &x );

	private:
		void		_getResrc( std::string absPath );
		void		_postResrc();
		void		_deleteResrc( std::string absPath );
		bool		_autoindexModule( std::string path );

		void		_extractData( void );
		
	public:
		void		errorMsg( std::string type );
		void		serveRequest( void );
		void		setData( Request req );
		void		setBytesSent(size_t bytesSent);
		void		offsetCursor(off_t offset);
		std::string	getBodyContent( void );
		void		headersSent();

	public:
		std::string	getHeaders( void ) const;
		std::string	getBody( void ) const;
		Request	getRequest( void ) const;
		std::string	getFileName( void ) const;
		std::string	getPath( void ) const;
		std::string	indexListContent( void ) const;
		int			getPos( void ) const;
		bool		getHeaderStatus( void ) const;
		bool		getStatus( void ) const;
		bool		isError( void ) const;
		bool		isAutoIndex( void ) const;
		size_t		getBodySize( void ) const;
		bool		bodyEof( void ) const;
		cgi			getCgi( void ) const;
		
	
	private:
		std::string			_headers;
		std::string			_body;
		std::string			_indexList;
		int					_bodyFd;
		size_t				_bodySize;
		size_t				_totalSent;
		bool				_headersSent;
		bool				_error;
		bool				_autoIndex;
		Request				_req;
		cgi					_cgi;
};

#endif // WEBSERV_RESPONSE_HPP
