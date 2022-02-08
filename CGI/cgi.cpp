/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-fadi <sel-fadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 12:52:15 by sel-fadi          #+#    #+#             */
/*   Updated: 2022/02/08 16:40:38 by sel-fadi         ###   ########.fr       */
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


char *const*setEnv(std::vector<std::string> my_env)
{
	char **env = new char*[my_env.size()];
	
	for (int i; i < my_env.size(); i++)
		env[i]= (char *)my_env[i].c_str();
	env[my_env.size() + 1] = NULL;
	return (env);
}

std::vector<std::string> setEnvInVector()
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

	my_env[0].append("74");
	my_env[1].append("Lorem Ipsum is simply dummy text of the printing and typesetting industry.");
	my_env[3].append("http://localhost/Users/sel-fadi/Desktop/webserv/cgi/test.php");
	my_env[4].append("--");
	my_env[5].append("--");
	my_env[6].append("POST");
	my_env[7].append("--");
	my_env[8].append("/test.php");
	my_env[9].append("--");
	my_env[10].append("--");
	return my_env;
}

int main()
{
	int rett;
	int fd[2];
	pid_t pid;
	int status;
	int ret;
	char **tmp;
	
	if (pipe(fd) == -1)
		exit(EXIT_FAILURE);
	std::vector<std::string> my_headers = setEnvInVector();
	char *const*env;
	tmp = new char*[3];
	std::string arg = "/Users/sel-fadi/Desktop/webserv/cgi/test.php";
	std::string scriptType = "/Users/sel-fadi/.brew/bin/php-cgi";
	// std::string arg = "/Users/sel-fadi/Desktop/webserv/cgi/test.py";
	// std::string scriptType = "/usr/bin/python";

	tmp[0] = strdup(scriptType.c_str());
	tmp[1] = strdup(arg.c_str());
	tmp[2] = NULL;
	env = setEnv(my_headers);
	
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	else if (pid == 0)
	{
		// I should exec cgi
		fd[1] = open("aliResponse.txt", O_WRONLY | O_CREAT, 0777);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
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
		wait(&status);
		if (!WEXITSTATUS(status))
			exit(EXIT_SUCCESS);
		else
			exit(EXIT_FAILURE);
	}
}