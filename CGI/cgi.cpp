/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-fadi <sel-fadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 12:52:15 by sel-fadi          #+#    #+#             */
/*   Updated: 2022/02/03 17:36:52 by sel-fadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <map>

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
    
    my_map.insert(std::make_pair("REDIRECT_STATUS", "200"));
    my_map.insert(std::make_pair("REQUEST_METHOD", "GET"));
    my_map.insert(std::make_pair("SCRIPT_FILENAME", arg));
    my_map.insert(std::make_pair("SCRIPT_NAME", "/test.py"));
    
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