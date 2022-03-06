/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi1.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-fadi <sel-fadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 12:17:14 by sel-fadi          #+#    #+#             */
/*   Updated: 2022/03/06 13:03:21 by sel-fadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

extern char **environ;
cgi::cgi()
{
    this->getOrPost = 1;
	this->documentOrRedirection = 0;
    this->queryString = "fname=heyyyyyyy";
    // std::string arg = "/Users/sel-fadi/Desktop/webserv/cgi/test.py";
	// std::string scriptType = "/usr/bin/python";
    this->arg = "/Users/sel-fadi/Desktop/OurWebserv/CGI/test.php";
    // this->arg = "/Users/sel-fadi/Desktop/OurWebserv/CGI/test1.php";
	this->scriptType = "/Users/sel-fadi/.brew/bin/php-cgi";
}

cgi::~cgi()
{
}

void cgi::setEnv(int getOrPost)
{
	if (getOrPost)
	{
		setenv("CONTENT_LENGTH","15", 1);
		setenv("SERVER_PROTOCOL","HTTP/1.1", 1);
		setenv("QUERY_STRING","fname=hey", 1);
		setenv("REQUEST_METHOD","POST", 1);
		setenv("REDIRECT_STATUS","200",1);
		setenv("CONTENT_TYPE", "application/x-www-form-urlencoded",1);
		setenv("SCRIPT_FILENAME", "/Users/sel-fadi/Desktop/OurWebserv/CGI/test.php",1);
	}
	else
	{
		setenv("SERVER_PROTOCOL","HTTP/1.1", 1);
		setenv("QUERY_STRING","fname=hey", 1);
		setenv("PATH_INFO","/test.php", 1);
		setenv("REQUEST_METHOD","get", 1);
		setenv("REDIRECT_STATUS","200",1);
		setenv("SCRIPT_FILENAME", "/Users/sel-fadi/Desktop/OurWebserv/CGI/test.php",1);
	}
}

void cgi::exec_script(int *fd, int *fd1)
{
    int ret;
    char *tmp[3];
    char *const*env;

	tmp[0] = (char*)scriptType.c_str();
	tmp[1] = (char*)arg.c_str();
	tmp[2] = NULL;

    if (this->getOrPost)
    { 
		setEnv(getOrPost);
        dup2(fd1[0], 0);
		// std::cout << this->queryString << "\n";
        write(fd1[1], this->queryString, strlen(this->queryString));
    }
    else
		setEnv(getOrPost);
	dup2(fd[1], STDOUT_FILENO); 
    close(fd[0]);
    close(fd1[0]);
    close(fd[1]);
    close(fd1[1]);
    ret = execve(tmp[0], tmp, environ);
    if (ret == -1)
        exit(EXIT_FAILURE);
}

void cgi::setHeader(const std::string &key, const std::string &value, bool end)
{
	this->_buffer += key + ": " + value + "\r\n";
	if (end)
		_buffer += "\r\n";
}
void cgi::handleResponse()
{
	this->_buffer.insert(0, "HTTP/1.1 \r\n");
	this->_buffer.insert(0, "Date: \r\n");
	this->_buffer.insert(0, "Accept-Ranges: none\r\n");
	this->_buffer.insert(0, "Server: webserv/1.1 \r\n");
}

void cgi::handleRedirectResponse()
{
	this->_buffer.insert(0, "Accept-Ranges: none\r\n");
	this->_buffer.insert(0, "Date: \r\n");
	this->_buffer.insert(0, "Server: webserv/1.1 \r\n");
	this->_buffer.insert(0, "HTTP/1.1 \r\n");
}

void cgi::script_output(int *fd, int *fd1, pid_t pid)
{
    int status;
    ssize_t count;
    char *buffer;

	if (documentOrRedirection)
		handleResponse();
	else
		handleRedirectResponse();
	buffer = (char *)malloc(10000);
    close(fd[1]);
    close(fd1[0]);
    close(fd1[1]);
	// bzero(buffer, 0);
    count = read(fd[0], buffer, 5000);
	// if (count < 1000)
	// 	read(fd[0], buffer, 1000 - count);
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
