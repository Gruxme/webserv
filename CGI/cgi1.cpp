/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi1.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-fadi <sel-fadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 12:17:14 by sel-fadi          #+#    #+#             */
/*   Updated: 2022/03/05 17:17:49 by sel-fadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

extern char **environ;
cgi::cgi()
{
    this->getOrPost = 1;
    // this->queryString = "fname=hey";
    std::string arg = "/Users/sel-fadi/Desktop/webserv/cgi/test.py";
	// std::string scriptType = "/usr/bin/python";
    this->arg = "/Users/sel-fadi/Desktop/OurWebserv/CGI/test.php";
    // // this->arg = "/Users/sel-fadi/Desktop/webserv/cgi/test.php";
	this->scriptType = "/Users/sel-fadi/.brew/bin/php-cgi";
    // this->my_headers = setEnvInVector(request);
	// this->my_headerPost = setEnvInVectorPost(request);
}

cgi::~cgi()
{
}

char *const* cgi::setEnv(std::vector<std::string> my_env)
{
	char **env = new char*[my_env.size()];
	
	for (int i = 0; i < my_env.size(); i++)
    {
		env[i] = (char *)my_env[i].c_str();
        std::cout << env[i] << std::endl;
    }
	env[my_env.size() + 1] = NULL;
	return (env);
}

std::vector<std::string> cgi::setEnvInVectorPost(Request &my_request)
{
	std::vector<std::string> my_env;
	my_env.push_back("CONTENT_LENGTH=");
	my_env.push_back("QUERY_STRING=");
	my_env.push_back("REQUEST_METHOD=POST");
	my_env.push_back("SCRIPT_FILENAME=");
	my_env.push_back("REDIRECT_STATUS=TRUE");

	my_env[0].append("9");
	my_env[1].append("fname=hey");
	my_env[3].append("/Users/sel-fadi/Desktop/OurWebserv/CGI/test.php");
	return my_env;
}

std::vector<std::string> cgi::setEnvInVector(Request &my_request)
{
	std::vector<std::string> my_env;
	// my_env.push_back("CONTENT_LENGTH=");
	// my_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	my_env.push_back("PATH_INFO=/test.php");
	// my_env.push_back("PATH_TRANSLATED=");
	my_env.push_back("QUERY_STRING=\"var1=value1\"");
	my_env.push_back("REQUEST_METHOD=GET");
	// my_env.push_back("REMOTE_ADDR=");
	my_env.push_back("SCRIPT_NAME=web");
	my_env.push_back("SERVER_NAME=ff");
	my_env.push_back("SERVER_PORT=8888");
	my_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	// setenv()
	my_env.push_back("SERVER_SOFTWARE=webserv");
	my_env.push_back("REDIRECT_STATUS=200");
	my_env.push_back("SCRIPT_FILENAME=test.php");

	// my_env[0].append(my_request.getHeaders().find("CONTENT_LENGTH")->second);
	// std::cout << my_request.getHeaders() << std::endl;
	// my_env[1].append("f");
	// my_env[3].append("/test.php");
	// my_env[4].append("--");
	// my_env[5].append("--");
	// my_env[6].append("GET");
	// my_env[7].append("--");
	// my_env[8].append("/test.php");
	// my_env[9].append("--");
	// my_env[10].append("--");
	// my_env[14].append("/Users/sel-fadi/Desktop/OurWebserv/CGI/test.php");
	return my_env;
}
     #include <stdlib.h>
void cgi::exec_script(int *fd, int *fd1)
{
    int ret;
    char *tmp[3];
    char *const*env;
    // tmp = new char*[3];
	tmp[0] = (char*)scriptType.c_str();
	tmp[1] = (char*)arg.c_str();
	tmp[2] = NULL;

    // if (this->getOrPost)
	// 	env = setEnv(this->my_headerPost);
	// else
	// 	env = setEnv(this->my_headers);
    // if (this->getOrPost)
    // {
		// setenv("CONTENT_LENGTH","9", 1);
		// setenv("SERVER_PROTOCOL","HTTP/1.1", 1);
		// setenv("QUERY_STRING","fname=hey", 1);
		// setenv("REQUEST_METHOD","POST", 1);
		// setenv("REDIRECT_STATUS","200",1);
		// setenv("CONTENT_TYPE", "text/html; charset=UTF-8",1);
		// setenv("SCRIPT_FILENAME", "/Users/sel-fadi/Desktop/OurWebserv/CGI/test.php",1);
        dup2(fd1[0], 0);
        write(fd1[1], this->queryString, strlen(this->queryString));
    // }
    // else
    // {
	// 	setenv("SERVER_PROTOCOL","HTTP/1.1", 1);
	// 	setenv("QUERY_STRING","v=iBy1Clvr_6o", 1);
	// 	setenv("PATH_INFO","/test.php", 1);
	// 	setenv("REQUEST_METHOD","get", 1);
	// 	setenv("REDIRECT_STATUS","200",1);
	// 	setenv("SCRIPT_FILENAME", "/Users/sel-fadi/Desktop/OurWebserv/CGI/test.php",1);
	// }
	dup2(fd[1], STDOUT_FILENO); 
    close(fd[0]);
    close(fd1[0]);
    close(fd[1]);
    close(fd1[1]);
	
	// std::cerr << tmp[0] << "  " << tmp[1] << "\n";
    ret = execve(tmp[0], tmp, environ);
    if (ret == -1)
        exit(EXIT_FAILURE);
}

void cgi::script_output(int *fd, int *fd1, pid_t pid)
{
    int status;
    ssize_t count;
    char buffer[4096];

    close(fd[1]);
    close(fd1[0]);
    close(fd1[1]);
    count = read(fd[0], &buffer, 4096);
	std::cout << buffer << std::endl;
    close(fd[0]);
    if (!WEXITSTATUS(status))
        exit(EXIT_SUCCESS);
    else
        exit(EXIT_FAILURE);
}

void cgi::processing_cgi(Request &request)
{
    int rett;
	int fd[2];
	int fd1[2];
    pid_t pid;

	setenv("CONTENT_LENGTH","9", 1);
	setenv("SERVER_PROTOCOL","HTTP/1.1", 1);
	setenv("QUERY_STRING","fname=hey", 1);
	setenv("REQUEST_METHOD","POST", 1);
	setenv("REDIRECT_STATUS","200",1);
	setenv("CONTENT_TYPE", "application/x-www-form-urlencoded",1);
	setenv("SCRIPT_FILENAME", "/Users/sel-fadi/Desktop/OurWebserv/CGI/test.php",1);
	if (pipe(fd) == -1)
		exit(EXIT_FAILURE);
	if (pipe(fd1) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	std::cout << "pid: " << pid << "\n";
	if (pid == -1)
		exit(EXIT_FAILURE);
	else if (pid == 0)
		exec_script(fd, fd1);
    else
        script_output(fd, fd1, pid);
	wait(NULL);
}
