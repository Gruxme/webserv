/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-fadi <sel-fadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 12:52:15 by sel-fadi          #+#    #+#             */
/*   Updated: 2022/02/03 13:09:19 by sel-fadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include <cstring>
int main()
{
    pid_t pid;
    int ret;
    char **tmp;
    tmp = new char*[2];
    std::string arg;
    arg = "/Users/sel-fadi/Desktop/webserv/cgi/test.py";
    tmp[0] = strdup(arg.c_str());
    tmp[1] = NULL;
    
    pid = fork();
    if (pid == -1)
        std::cout << "Something wrong in forking" << std::endl;
    else if (pid == 0)
    {
        // I should exec cgi
        std::cout << "Iam in the child process : " << const_cast<char *const*>(tmp)[0] << std::endl;
        ret = execve("/usr/bin/python3", tmp, NULL);
        if (ret == -1)
            std::cout << "Execve failed." << std::endl;
        else
            std::cout << "Execve Success." << std::endl;
        // std::cout << ret << std::endl;
    }
    else
    {
        // waiting the output of the child
        std::cout << "Iam in the parent process" << std::endl;
    }
}