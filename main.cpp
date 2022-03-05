/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 18:42:26 by abiari            #+#    #+#             */
/*   Updated: 2022/03/02 18:35:10 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./src/config/ConfigClass.hpp"
#include "./src/config/ServerConfigClass.hpp"
#include "./src/config/LocationClass.hpp"
#include "./src/request/Request.hpp"
#include "./src/sockets/sockets.hpp"
#include "./src/sockets/socketsIO.hpp"

bool g_sigpipe = false;

void	sigHandler(int sigNum)
{
	if(sigNum == SIGPIPE)
		g_sigpipe = true;
}

int	main(int argc, char **argv)
{
	ConfigClass	confFile;
	socketsIO	server;
	if (argc > 2) {
		std::cout << "Usage: ./webserv path_to_config" << std::endl;
		return (EXIT_FAILURE);
	}
	if(argc < 2)
		confFile = ConfigClass();
	if (argc == 2)
		confFile = ConfigClass(std::string(argv[1]));
	try {
		confFile.parseConfigFile();
	}
	catch (const std::exception& e)  {
		std::cerr << "Config syntax error: " << e.what() << '\n';
		return (EXIT_FAILURE);
	}

	signal(SIGPIPE, sigHandler);
	for (size_t i = 0; i < confFile.getServerCount(); i++) {
		//setter in socket to bind appropriate server config
		server.setSock(new sockets(confFile.getServerConfigClass()[i], 10));
	}
	server.eventListener();
	return (EXIT_SUCCESS);
}
