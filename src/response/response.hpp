/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/02/23 14:45:52 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include "../config/ConfigClass.hpp"
#include "../request/Request.hpp"

class response {
	public:
		response();
		response( const response &x );
		~response();
		response&	operator=( const response &x );

	private:
		void	_getResrc( std::string path );
		void	_postResrc( std::string path );
		void	_deleteResrc( std::string path );
		void	_autoindexModule( void );
		
	public:
		void		serveRequest( ServerConfigClass const &config, Request const &req );
		std::string	getMsg( void );
		
	private:
		std::string	res;
};

#endif // WEBSERV_RESPONSE_HPP
