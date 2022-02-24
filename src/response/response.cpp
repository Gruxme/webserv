/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/02/24 16:13:09 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"


response::response() {}
response::~response() {}

std::string	response::_errorMsg( std::string type ){
	std::ostringstream	errRes;
	errRes << "HTTP /1.1 " << type << "\nDate: ";
	time_t	now = time(0);
	char	*date = new char[30]();
	strftime(date, 29, "%a, %d %b %Y %T %Z", gmtime(&now));
	errRes << date << "\n" << "Server: Webserv/4.2.0 \n";
	
}

void response::_getResrc( std::string absPath ) {
    // (void)path;
	struct stat status;
	if (_req.getUriExtension() == PHP)
	{

	}
	else if(_req.getUriExtension() == PY){
		
	}
	else{
		_body.open(absPath, std::fstream::in);
		if(_body.fail()){
			// if(errno == ENOENT)
			// 	//send not found
			// else if(errno == EACCES)
			// 	//send forbidden
			// else
			//	//send 500 internal server error
		}
		if(stat(absPath.c_str(), &status) > 0 && S_ISDIR(status.st_mode)){
			// if(_config.getAutoIndex())
			// 	//launch autoindex module
			// else
			// 	//send forbidden
		}
		//
	}
    return ;
}

std::pair<int, std::string> response::_extractLocPosNAbsPath( void ) {
	std::string p = this->_req.getPath();
	
	return nullptr;
}

void response::serveRequest( void ) {
    std::pair<int, std::string> data = this->_extractLocPosNAbsPath();
    return ; 
}

void		response::setData( ServerConfigClass config, Request req ){
	_config = config;
	_req = req;
}
