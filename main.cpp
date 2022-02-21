/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 18:42:26 by abiari            #+#    #+#             */
/*   Updated: 2022/02/21 19:02:59 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/ConfigFile.hpp"
#include "config/ServerConfig.hpp"
#include "config/LocationClass.hpp"
#include "request/Request.hpp"
#include "sockets/sockets.hpp"
#include "sockets/socketsIO.hpp"

int	main(int argc, char **argv)
{
	ConfigFile	confFile;
	if(argc > 2)
		std::cout << "Usage: ./webserv path_to_config" << std::endl;
	if(argc == 2)
		confFile = ConfigFile(std::string(argv[1]));
	try{
		confFile.parseConfigFile();
	}
	catch(const std::exception& e){
		std::cerr << e.what() << '\n';
	}
	
	
}
