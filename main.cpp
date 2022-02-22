/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 18:42:26 by abiari            #+#    #+#             */
/*   Updated: 2022/02/22 14:04:05 by aabounak         ###   ########.fr       */
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
	socketsIO	server;
	if (argc > 2) {
		std::cout << "Usage: ./webserv path_to_config" << std::endl;
		return (EXIT_FAILURE);
	}
	if (argc == 2)
		confFile = ConfigFile(std::string(argv[1]));
	try {
		confFile.parseConfigFile();
	}
	catch (const std::exception& e)  {
		std::cerr << "Config syntax error: " << e.what() << '\n';
		return (EXIT_FAILURE);
	}

	for (size_t i = 0; i < confFile.getServerCount(); i++) {
		sockets	sock(confFile.getServerConfig()[i].getPort());
		sock.bindSock();
		sock.listener(10);
		//setter in socket to bind appropriate server config
		server.setSock(sock);
	}
	server.eventListener();
	return (EXIT_SUCCESS);
}
