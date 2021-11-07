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
	fd_set	readfds;

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
		FD_ZERO(&readfds);

		FD_SET(mainSock, &readfds);
		max_sd = mainSock;

		for (int i = 0; i < MAXCLIENTS; i++)
		{
			sd = clientSocks[i];

			if(sd > 0)
				FD_SET(sd, &readfds);

			if(sd > max_sd)
				max_sd = sd;
		}
		if((activity = select(max_sd + 1, &readfds, NULL, NULL, NULL)) < 0){
			perror("select err");
		}
		
		if(FD_ISSET(mainSock, &readfds)){
			if( (newSock = accept(mainSock, (SA *)&address, (socklen_t*)&addrlen)) < 0){
				perror("accept err");
				exit(EXIT_FAILURE);
			}
		}
		fcntl(newSock, F_SETFL, O_NONBLOCK);
		std::string	ipAddr(inet_ntoa(address.sin_addr));
		std::cout << "New connection, socket fd is: " << newSock << ", ip: " << ipAddr << ", port: " << ntohs(address.sin_port) << std::endl;
		char *msg = "ECHO Daemon v1.0 \r\n";
		if(send(newSock, msg, strlen(msg), 0) < 0){
			perror("send err");
		}

		std::cout << "Welcome msg sent\n";
		for (int i = 0; i < MAXCLIENTS; i++)
		{
			if(clientSocks[i] == 0){
				clientSocks[i] = newSock;
				std::cout << "adding to list of sockets as: " << i << std::endl;
				break; 
			}
		}
		
		for (int i = 0; i < MAXCLIENTS; i++)
		{
			sd = clientSocks[i];
			if(FD_ISSET(sd, &readfds)){
				if((valRead = read(sd, buff, 1024)) == 0){
					getpeername(sd, (SA *)&address, (socklen_t*)&addrlen);
					std::cout << "Host disconnected, ip: " << inet_ntoa(address.sin_addr) << ", port: " << ntohs(address.sin_port) << std::endl;
					close(sd);
					clientSocks[i] = 0;
				}
			}
			else{
				buff[valRead] = '\0';
				send(sd, buff, strlen(buff), 0);
			}
		}
	}
	return 0;
}
