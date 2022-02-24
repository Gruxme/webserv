/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/02/24 14:21:09 by abiari           ###   ########.fr       */
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

void response::_getResrc( std::string path ) {
    // (void)path;
	struct stat status;
	if (_req.getUriExtension() == PHP)
	{

	}
	else if(_req.getUriExtension() == PY){
		
	}
	else{
		_body.open(path, std::fstream::in);
		if(_body.fail()){
			// if(errno == ENOENT)
			// 	//send not found
			// else if(errno == EACCES)
			// 	//send forbidden
			// else
			//	//send 500 internal server error
		}
		if(stat(path.c_str(), &status) > 0 && S_ISDIR(status.st_mode)){
			// if(_config.getAutoIndex())
			// 	//launch autoindex module
			// else
			// 	//send forbidden
		}
		//
	}
    return ;
}

std::string response::_extractAbsolutePath( void ) {
    std::string s;
    std::string newUri;
    for (size_t i = 0; i < _config.getLocationCount(); i++) {
        s = _req.getUri();
        newUri = _req.getUri();
        for (int j = s.length(); j >= 0; j--) {
            if (s[j] == '/') {
                s = s.substr(0, j + 1);
                std::cout << s << std::endl;
                std::cout << _config.getLocationClass()[0].getPath() << std::endl;
                if (s == _config.getLocationClass()[i].getPath()) {
                    std::cout << "GOTCHA BITCH root-> " << _config.getLocationClass()[i].getRoot() << std::endl;
                    std::cout << "GOTCHA BITCH path-> " << _config.getLocationClass()[i].getPath() << std::endl;
                }
            }
        }
    }
    return s;
}

void response::serveRequest( void ) {
    std::string absolutePath = _extractAbsolutePath();
    return ; 
}

void		response::setData( ServerConfigClass config, Request req ){
	_config = config;
	_req = req;
}
