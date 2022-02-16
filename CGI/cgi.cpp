/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-fadi <sel-fadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 12:52:15 by sel-fadi          #+#    #+#             */
/*   Updated: 2022/02/16 12:27:03 by sel-fadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <map>
#include <cstdlib>
#include <vector>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include "../request/Request.hpp"

// Status codes
// 200 - OK
// 201 - OK created
// 301 - Moved to new URL
// 304 - Not Modified (Cached version)
// 400 - Bad request
// 401 - Unauthorized
// 404 - Not found
// 500 - Internal server error


// This Work
// REQUEST_METHOD=post
// REDIRECT_STATUS=TRUE
// QUERY_STRING=fname=hey
// CONTENT_LENGTH=9
// SCRIPT_FILENAME=/Users/sel-fadi/Desktop/webserv/cgi/test.php

// REQUEST_METHOD=POST
// REDIRECT_STATUS=true
// SCRIPT_FILENAME=/Users/sel-fadi/Desktop/webserv/cgi/test.php
// CONTENT_LENGTH=7
// QUERY_STRING=fname=a
// CONTENT_TYPE=text/html
// GATEWAY_INTERFACE=CGI/1.1
// SERVER_PROTOCOL=HTTP/1.1

char *const*setEnv(std::vector<std::string> my_env)
{
	char **env = new char*[my_env.size()];
	
	for (int i; i < my_env.size(); i++)
		env[i]= (char *)my_env[i].c_str();
	env[my_env.size() + 1] = NULL;
	return (env);
}

std::vector<std::string> setEnvInVectorPost(Request my_request)
{
	std::vector<std::string> my_env;
	my_env.push_back("CONTENT_LENGTH=");
	my_env.push_back("QUERY_STRING=");
	my_env.push_back("REQUEST_METHOD=post");
	my_env.push_back("SCRIPT_FILENAME=");
	my_env.push_back("REDIRECT_STATUS=TRUE");

	my_env[0].append("9");
	my_env[1].append("fname=hey");
	my_env[3].append("/Users/sel-fadi/Desktop/webserv/cgi/test1.php");
	return my_env;
}

std::vector<std::string> setEnvInVector(Request my_request)
{
	std::vector<std::string> my_env;
	my_env.push_back("CONTENT_LENGTH=");
	my_env.push_back("CONTENT_TYPE=");
	my_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	my_env.push_back("PATH_INFO=");
	my_env.push_back("PATH_TRANSLATED=");
	my_env.push_back("QUERY_STRING=");
	my_env.push_back("REQUEST_METHOD=");
	my_env.push_back("REMOTE_ADDR=");
	my_env.push_back("SCRIPT_NAME=");
	my_env.push_back("SERVER_NAME=");
	my_env.push_back("SERVER_PORT=");
	my_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	my_env.push_back("SERVER_SOFTWARE=webserv");
	my_env.push_back("REDIRECT_STATUS=200");

	// my_env[0].append(my_request.getHeaders().find("CONTENT_LENGTH")->second);
	// std::cout << my_request.getHeaders() << std::endl;
	my_env[1].append("Lorem Ipsum is simply dummy text of the printing and typesetting industry.");
	my_env[3].append("http://localhost/Users/sel-fadi/Desktop/webserv/cgi/test1.php");
	my_env[4].append("--");
	my_env[5].append("--");
	my_env[6].append("GET");
	my_env[7].append("--");
	my_env[8].append("/test.php");
	my_env[9].append("--");
	my_env[10].append("--");
	return my_env;
}

int main()
{
	Request my_request;
	int rett;
	int fd[2];
	int fd1[2];
	pid_t pid;
	int status;
	int ret;
	char **tmp;
	char *const*env;
	bool getOrPost = 1;
	char *queryString = "fname=hey";
	
	std::string arg = "/Users/sel-fadi/Desktop/webserv/cgi/test.php";
	std::string scriptType = "/Users/sel-fadi/.brew/bin/php-cgi";
	std::vector<std::string> my_headers = setEnvInVector(my_request);
	std::vector<std::string> my_headerPost = setEnvInVectorPost(my_request);
	if (getOrPost)
		env = setEnv(my_headerPost);
	else
		env = setEnv(my_headers);
	tmp = new char*[3];
	tmp[0] = strdup(scriptType.c_str());
	tmp[1] = strdup(arg.c_str());
	tmp[2] = NULL;
	if (pipe(fd) == -1)
		exit(EXIT_FAILURE);
	if (pipe(fd1) == -1)
		exit(EXIT_FAILURE);
	// std::string arg = "/Users/sel-fadi/Desktop/webserv/cgi/test.py";
	// std::string scriptType = "/usr/bin/python";
	
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	else if (pid == 0)
	{
		// I should exec cgi
		if (getOrPost)
		{
			fd1[1] = open("PostResponse.txt", O_WRONLY | O_CREAT, 0777);
			dup2(fd1[1], STDOUT_FILENO);
			write(fd1[1], queryString, strlen(queryString));
		}
		else
		{
			fd[1] = open("aliResponse.txt", O_WRONLY | O_CREAT, 0777);
			dup2(fd[1], STDOUT_FILENO);
		}
		close(fd[0]);
		close(fd1[0]);
		ret = execve(tmp[0], tmp, env);
		if (ret == -1)
			exit(EXIT_FAILURE);
		else
			exit(EXIT_SUCCESS);
	}
	else
	{
		char buffer[4096];

		close(fd[1]);
		ssize_t count = read(fd[0], &buffer, sizeof(buffer));
		close(fd[0]);
		close(fd1[0]);
		close(fd1[1]);
		wait(&status);
		if (!WEXITSTATUS(status))
			exit(EXIT_SUCCESS);
		else
			exit(EXIT_FAILURE);
	}
}