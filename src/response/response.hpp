/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/02/22 13:45:29 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP
#include "../config/ConfigFile.hpp"
#include "../request/Request.hpp"
class response {
	public:
		response();
		response(const response &x);
		~response();
		response&	operator=(const response &x);

		void	getResrc(std::string path);
		//check if file exists, if yes then check if it's a file or a folder
		//if file, get its data into a response with headers either raw 
		//or from cgi output if file is script, else call autoindexmodule
		void	postResrc(std::string path);
		void	deleteResrc(std::string path);
		void	serveRequest(Request req);
		void	autoindexModule(void);
	private:
		std::string	res;
};


#endif //WEBSERV_RESPONSE_HPP
