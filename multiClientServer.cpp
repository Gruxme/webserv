#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>

#define SERVER_PORT 8080
#define MAXLINE		4096
#define MAXCLIENTS	10
#define	SA	struct sockaddr

int	main()
{
	int opt = 1;
	int mainSock, addrlen, newSock, clientSocks[MAXCLIENTS];
	int	activity, valRead = 0, sd, max_sd;
	struct	sockaddr_in	address;
	char buff[1025];
	fd_set	readfds, writefds;

	bzero(&clientSocks, MAXCLIENTS);

	if((mainSock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket create err");
		exit(EXIT_FAILURE);
	}
	fcntl(mainSock, F_SETFL, O_NONBLOCK);
	if(setsockopt(mainSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
		perror("setsockopt err");
		exit(EXIT_FAILURE);
	}
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(SERVER_PORT);
	address.sin_family = AF_INET;

	if(bind(mainSock, (SA *) &address, sizeof(address)) < 0){
		perror("bind err");
		exit(EXIT_FAILURE);
	}

	std::cout << "Socket listening on port " << SERVER_PORT << std::endl;
	if(listen(mainSock, 10) < 0){
		perror("listen err");
		exit(EXIT_FAILURE);
	}
	addrlen = sizeof(address);
	std::cout << "Waiting for connections ...\n";

	while (1)
	{
		bzero(&buff, 1025);
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		newSock = 0;
		FD_SET(mainSock, &readfds);
		max_sd = mainSock;

		for (int i = 0; i < MAXCLIENTS; i++)
		{
			sd = clientSocks[i];

			if(sd > 0){
				FD_SET(sd, &readfds);
				FD_SET(sd, &writefds);
			}

			if(sd > max_sd)
				max_sd = sd;
		}
		if((activity = select(max_sd + 1, &readfds, &writefds, NULL, NULL)) < 0){
			perror("select err");
		}
		
		if(FD_ISSET(mainSock, &readfds)){
			if( (newSock = accept(mainSock, (SA *)&address, (socklen_t*)&addrlen)) < 0){
				perror("accept err");
				exit(EXIT_FAILURE);
			}
		}
		fcntl(newSock, F_SETFL, O_NONBLOCK);
		if(newSock){
			std::string ipAddr(inet_ntoa(address.sin_addr));
			std::cout << "New connection, socket fd is: " << newSock << ", ip: " << ipAddr << ", port: " << ntohs(address.sin_port) << std::endl;
			// std::string	msg("Connected to server \r\n");
			// if (send(newSock, msg.c_str(), msg.length(), 0) < 0)
			// 	perror("send err");
			// std::cout << "Welcome msg sent\n";
			for (int i = 0; i < MAXCLIENTS; i++)
			{
				if (clientSocks[i] == 0)
				{
					clientSocks[i] = newSock;
					std::cout << "added to list of sockets as: " << i << std::endl;
					break;
				}
			}
			// continue ;
		}
		
		for (int i = 0; i < MAXCLIENTS; i++)
		{
			sd = clientSocks[i];
			std::cout << "socket: " << sd << "isset= " << FD_ISSET(sd, &readfds) << std::endl;
			if(FD_ISSET(sd, &readfds)){
				if((valRead = recv(sd, buff, 1024, 0)) == 0){
					getpeername(sd, (SA *)&address, (socklen_t*)&addrlen);
					std::cout << "Host disconnected, ip: " << inet_ntoa(address.sin_addr) << ", port: " << ntohs(address.sin_port) << std::endl;
					close(sd);
					clientSocks[i] = 0;
				}
				buff[valRead] = '\0';
				std::cout << "----begin req----\n";
				std::cout << buff;
				std::cout << "----end req----" << std::endl;
				break ;
			}
		}
		for (int i = 0; i < MAXCLIENTS; i++)
		{
			std::string buffer("HTTP/1.1 200 OK\nConnection: close\nContent-Type: text/html\nContent-Length: 306\n\n<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"https://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\">\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n<title>Webserv</title>\n<body>\n<h1>Connected to server</h1>\n</body>\n</html>");
			sd = clientSocks[i];
			if(FD_ISSET(sd, &writefds)){
				send(sd, buffer.c_str(), buffer.length(), 0);
				close(sd);
				clientSocks[i] = 0;
				std::cout << "response sent\n";
				break ;
			}
		}
		
	}
	return 0;
}
