/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/02/22 15:45:24 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

response::response() {}
response::~response() {}

void response::__getResrc( std::string path ) {
    
    return ;
}

void response::serveRequest( ServerConfig config, Request req ) {
    for (size_t i = 0; i < config.getLocationCount(); i++) {
        std::string s = req.getUri();
        for (int i = s.length(); i >= 0; i--) {
            if (s[i] == '/') {
                s = s.substr(0, i + 1);
                if (s == config.getLocationClass()[i].getPath()) {
                    std::cout << "GOTCHA BITCH root-> " << config.getLocationClass()[i].getRoot() << std::endl;
                    std::cout << "GOTCHA BITCH path-> " << config.getLocationClass()[i].getPath() << std::endl;
                }
            }
        }
        
    }
    return ;
}
