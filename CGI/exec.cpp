#include <iostream>
#include <unistd.h>
#include <cstring>
int main()
{
    pid_t pid;
    int ret;
    char **tmp;
    tmp = (char **)malloc(sizeof(char *) * 2);
    std::string arg;
    arg = "-la";
    tmp[0] = strdup(arg.c_str());
    tmp[1] = NULL;
    pid = fork();
    if (pid == -1)
        std::cout << "Something wrong in forking" << std::endl;
    else if (pid == 0)
    {
        // I should exec cgi
        // std::cout << "Iam in the child process : " << const_cast<char *const*>(tmp)[0] << std::endl;
        ret = execve("/bin/ls", tmp, NULL);
        std::cout << "tmp: " << tmp[0] << std::endl;
        if (ret == -1)
            perror("execve failed:");
        else
            std::cout << "Execve Success." << std::endl;
    }
    else
    {
        // waiting the output of the child
        std::cout << "Iam in the parent process" << std::endl;
    }
}