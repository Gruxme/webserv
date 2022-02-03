/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-fadi <sel-fadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 12:52:15 by sel-fadi          #+#    #+#             */
/*   Updated: 2022/02/03 20:30:43 by sel-fadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <map>
#include <cstdlib>
#include <vector>


const std::string ENV[4] = {
    "QUERY_STRING", "REQUEST_METHOD", "SCRIPT_FILENAME", "SCRIPT_NAME"
};

void setEnv()
{
    std::vector<std::string> my_env;
	my_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	my_env.push_back("SERVER_SOFTWARE=webserv");
	my_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	my_env.push_back("CONTENT_LENGTH=");
	my_env.push_back("CONTENT_TYPE=");
	my_env.push_back("PATH_INFO=");
	my_env.push_back("QUERY_STRING=");
	my_env.push_back("REQUEST_METHOD=");
	my_env.push_back("REMOTE_ADDR=");
	my_env.push_back("SCRIPT_NAME=");
	my_env.push_back("SERVER_NAME=");
	my_env.push_back("SERVER_PORT=");
	my_env.push_back("PATH_TRANSLATED=");
	my_env.push_back("REDIRECT_STATUS=200");
    
    my_env[3].append("99");
    std::cout << my_env[3] <<  std::endl;
}

int main()
{
    std::map<char **, std::string> my_map;
    int fd[2];
    pid_t pid;
    int ret;
    char **tmp;
    char **env;
    tmp = new char*[3];
    std::string arg = "/Users/sel-fadi/Desktop/webserv/cgi/test.py";
    std::string scriptType = "/usr/bin/python";

    tmp[0] = strdup(scriptType.c_str());
    tmp[1] = strdup(arg.c_str());
    tmp[2] = NULL;
    
    // my_map.insert(std::make_pair("REDIRECT_STATUS", "200"));
    // my_map.insert(std::make_pair("REQUEST_METHOD", "GET"));
    // my_map.insert(std::make_pair("SCRIPT_FILENAME", arg));
    // my_map.insert(std::make_pair("SCRIPT_NAME", "/test.py"));
    

    // ENV
    for (int i = 0; i < 4; i++)
    {
        // std::cout << "<tr><td>" << ENV[i] << "</td><td>" << std::endl;
        std::cout << ENV[i] << std::endl;
        std::cout << std::getenv(ENV[i].c_str()) << std::endl;
        const char *value = std::getenv(ENV[i].c_str());
        if (value != 0)
            std::cout << value;
        else
            std::cout << "Environment variables does not exist." << std::endl;
        // std::cout << "</td></tr>\n";
    }
    // std::cout << ;
    // std::cout << ;
    pid = fork();
    if (pid == -1)
        std::cout << "Something wrong in forking" << std::endl;
    else if (pid == 0)
    {
        // I should exec cgi
        std::cout << "Iam in the child process." << std::endl;
        ret = execve(tmp[0], tmp, NULL);
        if (ret == -1)
            std::cout << "Execve failed." << std::endl;
        else
            std::cout << "Execve Success." << std::endl;
    }
    else
    {
        // waiting the output of the child
        std::cout << "Iam in the parent process" << std::endl;
        // close(fd[0]);
        // close(fd[1]);
    }
}

// int main()
// {
//     setEnv();
//     return 0;
// }