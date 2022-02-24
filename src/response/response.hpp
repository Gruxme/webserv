/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/02/24 15:24:40 by aabounak         ###   ########.fr       */
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
		std::string	_errorMsg( std::string type );
		void		_getResrc( std::string data );
		void		_postResrc( std::string data );
		void		_deleteResrc( std::string data );
		void		_autoindexModule( void );

		void		_extractData( void );
		
	public:
		void		serveRequest( void );
		void		setData(ServerConfigClass config, Request req);
		
	private:
		std::string			_headers;
		std::fstream		_body;
		ServerConfigClass	_config;
		Request				_req;

	private:
		std::string	_scriptName;
		std::string	_path;
		int			_pos;
		
};

#endif // WEBSERV_RESPONSE_HPP
