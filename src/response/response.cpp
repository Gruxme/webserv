/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/02/23 18:01:14 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

response::response() {}
response::~response() {}

void response::_getResrc( std::string path ) {
    // (void)path;
	if (_req.getUriExtension() == PHP)
	{

	}
	else if(_req.getUriExtension() == PY){
		
	}
	else{
		
	}
    return ;
}

void response::serveRequest( void ) {
    for (size_t i = 0; i < _config.getLocationCount(); i++) {
        std::string s = _req.getUri();
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
    return ; 
}

void		response::setData(ServerConfigClass config, Request req){
	_config = config;
	_req = req;
}
