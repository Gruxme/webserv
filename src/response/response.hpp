/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/02/22 15:40:43 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include "../config/ConfigFile.hpp"
#include "../request/Request.hpp"

class response {
	public:
		response();
		response( const response &x );
		~response();
		response&	operator=( const response &x );

	private:
		void	__getResrc( std::string path );
		void	__postResrc( std::string path );
		void	__deleteResrc( std::string path );
		void	__autoindexModule( void );
		
	public:
		void	serveRequest( ServerConfig config, Request req );
		
	private:
		std::string	res;
};

#endif // WEBSERV_RESPONSE_HPP
