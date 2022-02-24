/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi1.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-fadi <sel-fadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 12:17:14 by sel-fadi          #+#    #+#             */
/*   Updated: 2022/02/24 14:45:05 by sel-fadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

cgi::cgi()
{
    this->getOrPost = 1;
    this->queryString = "fname=hey";
    // std::string arg = "/Users/sel-fadi/Desktop/webserv/cgi/test.py";
	// std::string scriptType = "/usr/bin/python";
    // this->arg = "/Users/sel-fadi/Desktop/webserv/cgi/test1.php";
    this->arg = "/Users/sel-fadi/Desktop/webserv/cgi/test.php";
	this->scriptType = "/Users/sel-fadi/.brew/bin/php-cgi";
    this->my_headers = setEnvInVector(request);
	this->my_headerPost = setEnvInVectorPost(request);
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
	my_env[3].append("/Users/sel-fadi/Desktop/webserv/cgi/test.php");
	return my_env;
}

std::vector<std::string> cgi::setEnvInVector(Request &my_request)
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
	my_env[3].append("http://localhost/Users/sel-fadi/Desktop/webserv/cgi/test.php");
	my_env[4].append("--");
	my_env[5].append("--");
	my_env[6].append("GET");
	my_env[7].append("--");
	my_env[8].append("/test.php");
	my_env[9].append("--");
	my_env[10].append("--");
	return my_env;
}

void cgi::exec_script(int *fd, int *fd1)
{
    int ret;
    char **tmp;
    char *const*env;

    tmp = new char*[3];
	tmp[0] = strdup(scriptType.c_str());
	tmp[1] = strdup(arg.c_str());
	tmp[2] = NULL;
    if (this->getOrPost)
		env = setEnv(this->my_headerPost);
	else
		env = setEnv(this->my_headers);
    if (this->getOrPost)
    {
        fd1[1] = open("PostResponse.txt", O_WRONLY | O_CREAT, 0777);
        dup2(fd1[1], STDOUT_FILENO);
        write(fd1[1], this->queryString, strlen(this->queryString));
    }
    else
    {
        fd[1] = open("GetResponse.txt", O_WRONLY | O_CREAT, 0777);
        dup2(fd[1], STDOUT_FILENO);
    }
    close(fd[0]);
    close(fd1[0]);
    ret = execve(tmp[0], tmp, env);
    // Status = 500
    if (ret == -1)
        exit(EXIT_FAILURE);
    else
        exit(EXIT_SUCCESS);
}

void cgi::script_output(int *fd, int *fd1, pid_t pid)
{
    int status;
    ssize_t count;
    char buffer[4096];

    close(fd[1]);
    fd[0] = open("GetResponse.txt", O_RDONLY);
    count = read(fd[0], &buffer, sizeof(buffer));
    sleep(2);
    kill(pid, SIGKILL);
    close(fd[0]);
    close(fd1[0]);
    close(fd1[1]);
    wait(&status);
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
}
