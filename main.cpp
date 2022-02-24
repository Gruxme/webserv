/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 18:42:26 by abiari            #+#    #+#             */
/*   Updated: 2022/02/23 14:04:23 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./src/config/ConfigClass.hpp"
#include "./src/config/ServerConfigClass.hpp"
#include "./src/config/LocationClass.hpp"
#include "./src/request/Request.hpp"
#include "./src/sockets/sockets.hpp"
#include "./src/sockets/socketsIO.hpp"

int	main(int argc, char **argv)
{
	ConfigClass	confFile;
	socketsIO	server;
	if (argc > 2) {
		std::cout << "Usage: ./webserv path_to_config" << std::endl;
		return (EXIT_FAILURE);
	}
	if (argc == 2)
		confFile = ConfigClass(std::string(argv[1]));
	try {
		confFile.parseConfigFile();
	}
	catch (const std::exception& e)  {
		std::cerr << "Config syntax error: " << e.what() << '\n';
		return (EXIT_FAILURE);
	}

	for (size_t i = 0; i < confFile.getServerCount(); i++) {
		sockets	sock(confFile.getServerConfigClass()[i]);
		sock.bindSock();
		sock.listener(10);
		//setter in socket to bind appropriate server config
		server.setSock(sock);
	}
	server.eventListener();
	return (EXIT_SUCCESS);
}
