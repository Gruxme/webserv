/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/02/24 16:25:00 by aabounak         ###   ########.fr       */
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

void	response::_extractData( void ) {
	std::string tmpPath = this->_req.getPath();
	std::string path = this->_req.getPath();
	while (path.find("/") != std::string::npos) {
		_path = path;
		for (int i = 0; i < _config.getLocationCount(); i++) {
			if ("/" == _config.getLocationClass()[i].getPath() && _config.getLocationClass()[i].getMethod() == _req.getMethod())
				this->_pos = i;
			else if (path == _config.getLocationClass()[i].getPath() && _config.getLocationClass()[i].getMethod() == _req.getMethod() ) {
				this->_scriptName = tmpPath.substr(this->_path.length(), tmpPath.length());
				if (this->_scriptName == "/")
					this->_scriptName = "";
					
				else if (this->_scriptName.find("/") != std::string::npos) {
					this->_scriptName.erase(this->_scriptName.find_first_of("/"));
				}
				this->_pos = i;
				return ;
			}
		}
		if (path.find_first_of("/") == path.find_last_of("/")) {
			this->_path = "/";
			this->_scriptName = tmpPath.substr(tmpPath.find("/") + 1, tmpPath.length());
			return ;
		}
		else
			path = path.substr(0, path.find_last_of("/"));
	}
	return ;
}

void response::serveRequest( void ) {
	this->_extractData();
    return ; 
}

void		response::setData( ServerConfigClass config, Request req ){
	_config = config;
	_req = req;
}
