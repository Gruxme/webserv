/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testMain.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 11:26:34 by aabounak          #+#    #+#             */
/*   Updated: 2022/03/05 11:10:00 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ---------------------------------------------------------------- */

/* SOCKETS MAIN -- DO NOT TOUCH {GRUXME ALLOWED} */

/* #include "./src/sockets/socketsIO.hpp"
#include "./src/sockets/sockets.hpp"

int	main()
{
	sockets		sock(4242);
	socketsIO	server;
	// sock.setNonBlock();
	sock.bindSock();
	sock.listener(10);
	server.setSock(sock);
	server.eventListener();
}
 */

/* ---------------------------------------------------------------- */

# include "./src/config/ConfigClass.hpp"
# include "./src/config/ServerConfigClass.hpp"
# include "./src/config/LocationClass.hpp"
# include <stdio.h>
# include <stdlib.h>
# include "./src/request/Request.hpp"
// # include "./src/response/response.hpp"

// std::string _generateRandomFilename( void ) {
//     char *s = (char *)malloc(10 + 1);
//     std::fstream ss;
//     ss.open("/dev/random");
//     ss.read(s, 10);
//     s[strlen(s) - 1] = '\0';
//     ss.close();
//     return s;
// }

// int main( void ) {
//     std::string s = _generateRandomFilename();
//     std::cout << s << std::endl;
// }

// /* CONFIGURATION FILE MAIN -- DO NOT TOUCH */
// int main( int ac, char **av ) {

//     (void)ac;
//     (void)av;

//     ConfigClass s = ConfigClass();
    // s.parseConfigFile();
    // std::cout << "-------- server 1 --------" << std::endl;
    // std::cout << s.getServerConfigClass()[0].getPort() << std::endl;
    // std::cout << s.getServerConfigClass()[0].getServerName() << std::endl;
    // std::cout << s.getServerConfigClass()[0].getRedirect() << std::endl;
    // std::cout << s.getServerConfigClass()[0].getBodySizeLimit() << std::endl;
    // std::cout << s.getServerConfigClass()[0].getAccessLog() << std::endl;
    // std::cout << s.getServerConfigClass()[0].getErrorPage() << std::endl;
    // std::cout << s.getServerConfigClass()[0].getAutoIndex() << std::endl;
    // std::cout << "-------- server 1 | location 1 --------" << std::endl;
    // std::cout << s.getServerConfigClass()[0].getLocationClass()[0].getPath() << std::endl;
    // std::cout << s.getServerConfigClass()[0].getLocationClass()[0].getRoot() << std::endl;
    // std::cout << s.getServerConfigClass()[0].getLocationClass()[0].getMethod() << std::endl;
    // std::cout << s.getServerConfigClass()[0].getLocationClass()[0].getRedirect() << std::endl;
    // std::cout << s.getServerConfigClass()[0].getLocationClass()[0].getCgiExt() << std::endl;
    // std::cout << "-------- server 1 | location 2 --------" << std::endl;
    // std::cout << s.getServerConfigClass()[0].getLocationClass()[1].getPath() << std::endl;
    // std::cout << s.getServerConfigClass()[0].getLocationClass()[1].getRoot() << std::endl;
    // std::cout << s.getServerConfigClass()[0].getLocationClass()[1].getCgiExt() << std::endl;
    // std::cout << std::endl;
    // std::cout << "-------- server 2 --------" << std::endl;
    // std::cout << s.getServerConfigClass()[1].getPort() << std::endl;
    // std::cout << s.getServerConfigClass()[1].getServerName() << std::endl;
    // std::cout << s.getServerConfigClass()[1].getRedirect() << std::endl;
    // std::cout << s.getServerConfigClass()[1].getBodySizeLimit() << std::endl;
    // std::cout << s.getServerConfigClass()[1].getAccessLog() << std::endl;
    // std::cout << s.getServerConfigClass()[1].getErrorPage() << std::endl;
    // std::cout << s.getServerConfigClass()[1].getAutoIndex() << std::endl;
    // std::cout << "-------- server 1 | location 1 --------" << std::endl;
    // std::cout << s.getServerConfigClass()[1].getLocationClass()[0].getPath() << std::endl;
    // std::cout << s.getServerConfigClass()[1].getLocationClass()[0].getRoot() << std::endl;
    // std::cout << s.getServerConfigClass()[1].getLocationClass()[0].getMethod() << std::endl;
    // std::cout << s.getServerConfigClass()[1].getLocationClass()[0].getRedirect() << std::endl;
    // std::cout << s.getServerConfigClass()[1].getLocationClass()[0].getCgiExt() << std::endl;
    // std::cout << "-------- server 1 | location 2 --------" << std::endl;
    // std::cout << s.getServerConfigClass()[1].getLocationClass()[1].getPath() << std::endl;
    // std::cout << s.getServerConfigClass()[1].getLocationClass()[1].getRoot() << std::endl;
    // std::cout << s.getServerConfigClass()[1].getLocationClass()[1].getCgiExt() << std::endl;

//     return EXIT_SUCCESS;
// }

# include "SimpleSocket.hpp"

# define BUFFER_SIZE 1024

int main( void ) {
    ConfigClass config = ConfigClass();
    config.parseConfigFile();
    SimpleSocket socket = SimpleSocket(AF_INET, SOCK_STREAM, 0);
    std::string str = "Le serveur vous envoie un bonsoir!\n";
    socket.bind();
    socket.listen();
    try {
        while (420) {
            Request req = Request();
            // response res = response();
            std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
            int newSocket = socket.accept();
            char buffer[BUFFER_SIZE] = {0};
            recv(newSocket, buffer, BUFFER_SIZE, 0);

            /* -- INVOKING PARSER ---------- */
            req.append(buffer);
            req.parse();

            // std::cout << req << std::endl;
// 
            // res.setData(config.getServerConfigClass()[0], req);
            // res.serveRequest();

            // std::cout << res.getFileName() << std::endl;
            // std::cout << res.getPath() << std::endl;
            // std::cout << res.getPos() << std::endl;

            /* ------------------------------ */
            // std::cout << "------------------ Message sent -------------------" << std::endl;
            // write(newSocket, str.c_str(), str.length());
            close(newSocket);
        }
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        socket.close();
        exit(0);
    }
    
    return (EXIT_SUCCESS);
}


/* REQUEST MAIN -- DO NOT TOUCH */
// int main( int ac, char **av ) {

//     (void)ac;
//     (void)av;

//     Request s = Request();
//     s.parse();
//     // std::cout << std::endl << "------ basic request _dataGatherer -----" << std::endl << std::endl;
//     // std::cout << s.getDataGatherer() << std::endl;
//     // std::cout << "------ request line extraction ------" << std::endl << std::endl;
//     // std::cout << s.getMethod() << std::endl;
//     // std::cout << s.getUri() << std::endl;
//     // std::cout << s.getProtocol() << std::endl;
//     // std::cout << s.getUriExtension() << std::endl;
//     // std::cout << std::endl << "------ extract headers ------" << std::endl << std::endl;
//     // std::map<std::string, std::string>::iterator it;
//     // for (it = s.getHeaders().begin(); it != s.getHeaders().end(); it++) {
//     //     std::cout << it->first << " : " << it->second << std::endl;
//     // }
//     // std::cout << std::endl << "------ extract body/content ------" << std::endl << std::endl;
//     // std::cout << "Body --> " << s.getBodyFilename() << std::endl << std::endl;
//     return EXIT_SUCCESS;
// }


/* VECTOR MAIN -- DO NOT TOUCH */
// class m
// {
//     private:
        
//     public:
//         m() {}
//         ~m() {}
// };

// int main( void ) {
//     std::vector<m> _vec;
//     for (int i = 0; i < 4; i++) {
//         _vec.push_back(m());
//     }
// }


/* REGEX MAIN -- HEX TEST MAIN -- DO NOT TOUCH */
/* bool is_hex_notation( std::string const& s )
{
    return s.compare(0, 2, "0x") == 0
      && s.size() > 2
      && s.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
}

int main( void )
{
    std::string s = "0x7DDD\r\n";
    s.erase(s.find("\r\n"));
    if (is_hex_notation(s))
        std::cout << s << std::endl;
    else
        std::cout << "WTF" << std::endl;
} */