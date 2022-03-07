#pragma once
#include <string>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>


class SimpleSocket
{
    // Will become private in a minute
    private:
        struct sockaddr_in  _address;
        int _addressLen;
        int _socketFd;

    protected:
        SimpleSocket() {}
        
    public:
        SimpleSocket( int domain, int type, int protocol ) {
            this->_socketFd = ::socket(domain, type, protocol);
            if (this->_socketFd < 0) {
                std::cerr << "Opening socket error: " << strerror(errno) << std::endl;
                exit(EXIT_FAILURE);
            }
            this->_addressLen = sizeof(this->_address); 
            this->_address.sin_family = domain;
            this->_address.sin_addr.s_addr = INADDR_ANY;
            this->_address.sin_port = htons( 1337 );
            memset(this->_address.sin_zero, '\0', sizeof(this->_address.sin_zero));
            int flag = 1;
            if (setsockopt(this->_socketFd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1)
                std::cerr << "Setsockopt fail" << std::endl;
        }

        int     bind() {
            int status = ::bind(this->_socketFd, (struct sockaddr *)&this->_address, this->_addressLen);
            if (status < 0) {
                std::cerr << "bind error: " << strerror(errno) << std::endl;
                exit(EXIT_FAILURE);
            }
            return status;
        }
        int     listen() { 
            int status = ::listen(this->_socketFd, 10);
            if (status < 0) {
                std::cerr << "listen error: " << strerror(errno) << std::endl;
                exit(EXIT_FAILURE);
            }
            return status;
        }
        int     accept() {
            int status = ::accept(this->_socketFd, (struct sockaddr *)&this->_address, (socklen_t *)&this->_addressLen);
            if (status < 0) {
                std::cerr << "accept error: " << strerror(errno) << std::endl;
                exit(EXIT_FAILURE);
            }
            return status;
        }
        void    close() { ::close(this->_socketFd); }

        ~SimpleSocket() {}

        

/*         int getSocket( void ) const { return this->_socketFd; }
        int getAddrLen( void ) const { return this->_addressLen; } */
        
};