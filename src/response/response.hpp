/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/02/24 14:30:16 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include "../config/ConfigClass.hpp"
#include "../request/Request.hpp"
#include <sys/stat.h>
#include <ctime>


class response {
	public:
		response();
		response( const response &x );
		~response();
		response&	operator=( const response &x );

	private:
		std::string	_errorMsg(std::string type);
		void		_getResrc( std::string absPath );
		void		_postResrc( std::string absPath );
		void		_deleteResrc( std::string absPath );
		void		_autoindexModule( void );

		std::string	_extractAbsolutePath( void );
		
	public:
		void		serveRequest( void );
		void		setData(ServerConfigClass config, Request req);
		
	private:
		std::string			_headers;
		std::fstream		_body;
		ServerConfigClass	_config;
		Request				_req;
		
};

#endif // WEBSERV_RESPONSE_HPP
