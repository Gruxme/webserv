/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 19:22:03 by aabounak          #+#    #+#             */
/*   Updated: 2021/12/20 22:52:56 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "SimpleSocket.hpp"

# define BUFFER_SIZE 1024

int main( void ) {
    
    SimpleSocket socket = SimpleSocket(AF_INET, SOCK_STREAM, 0);
    std::string str = "Le serveur vous envoie un bonsoir!\n";
    socket.bind();
    socket.listen();
    while (420) {
        std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
        int newSocket = socket.accept();
        char buffer[BUFFER_SIZE] = {0};
        recv(newSocket, buffer, BUFFER_SIZE, 0);
        std::cout << buffer << std::endl;
        write(newSocket, str.c_str(), str.length());
        std::cout << "------------------ Message sent -------------------" << std::endl;
        close(newSocket);
    }
    return (EXIT_SUCCESS);
}