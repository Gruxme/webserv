/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/02/23 14:48:59 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

response::response() {}
response::~response() {}

void response::_getResrc( std::string path ) {
    (void)path;
    return ;
}

void response::serveRequest( ServerConfigClass const &config, Request const &req ) {
    for (size_t i = 0; i < config.getLocationCount(); i++) {
        std::string s = req.getUri();
        for (int j = s.length(); j >= 0; j--) {
            if (s[j] == '/') {
                s = s.substr(0, j + 1);
                std::cout << s << std::endl;
                std::cout << config.getLocationClass()[0].getPath() << std::endl;
                if (s == config.getLocationClass()[i].getPath()) {
                    std::cout << "GOTCHA BITCH root-> " << config.getLocationClass()[i].getRoot() << std::endl;
                    std::cout << "GOTCHA BITCH path-> " << config.getLocationClass()[i].getPath() << std::endl;
                }
            }
        }
    }
    return ; 
}
