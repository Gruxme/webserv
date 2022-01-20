/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 17:42:56 by aabounak          #+#    #+#             */
/*   Updated: 2021/12/24 12:36:21 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "header.hpp"

class SimpleSocket
{
    // Will become private in a minute
    private:
        struct sockaddr_in  __address;
        int __addressLen;
        int __socketFd;

    protected:
        SimpleSocket() {}
        
    public:
        SimpleSocket( int domain, int type, int protocol ) {
            this->__socketFd = ::socket(domain, type, protocol);
            if (this->__socketFd < 0) {
                std::cerr << "Opening socket error: " << strerror(errno) << std::endl;
                exit(EXIT_FAILURE);
            }
            this->__addressLen = sizeof(this->__address); 
            this->__address.sin_family = domain;
            this->__address.sin_addr.s_addr = INADDR_ANY;
            this->__address.sin_port = htons( PORT );
            memset(this->__address.sin_zero, '\0', sizeof(this->__address.sin_zero));
        }

        int     bind() {
            int status = ::bind(this->__socketFd, (struct sockaddr *)&this->__address, this->__addressLen);
            if (status < 0) {
                std::cerr << "bind error: " << strerror(errno) << std::endl;
                exit(EXIT_FAILURE);
            }
            return status;
        }
        int     listen() { 
            int status = ::listen(this->__socketFd, 10);
            if (status < 0) {
                std::cerr << "listen error: " << strerror(errno) << std::endl;
                exit(EXIT_FAILURE);
            }
            return status;
        }
        int     accept() {
            int status = ::accept(this->__socketFd, (struct sockaddr *)&this->__address, (socklen_t *)&this->__addressLen);
            if (status < 0) {
                std::cerr << "accept error: " << strerror(errno) << std::endl;
                exit(EXIT_FAILURE);
            }
            return status;
        }
        void    close() { ::close(this->__socketFd); }

        ~SimpleSocket() {}

        

/*         int getSocket( void ) const { return this->__socketFd; }
        int getAddrLen( void ) const { return this->__addressLen; } */
        
};