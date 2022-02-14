/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 11:26:34 by aabounak          #+#    #+#             */
/*   Updated: 2022/02/14 16:03:08 by aabounak         ###   ########.fr       */
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

# include "./src/config/ConfigFile.hpp"
# include "./src/config/ServerConfig.hpp"
# include "./src/config/LocationClass.hpp"
# include "./src/request/Request.hpp"

/* CONFIGURATION FILE MAIN -- DO NOT TOUCH */
/* int main( int ac, char **av ) {

    (void)ac;
    (void)av;

    ConfigFile s = ConfigFile();
    s.parseConfigFile();
    std::cout << "-------- server 1 --------" << std::endl;
    std::cout << s.__serverConf[0].__port << std::endl;
    std::cout << s.__serverConf[0].__serverName << std::endl;
    std::cout << s.__serverConf[0].__host << std::endl;
    std::cout << s.__serverConf[0].__bodySizeLimit << std::endl;
    std::cout << s.__serverConf[0].__accessLog << std::endl;
    std::cout << s.__serverConf[0].__errorPage << std::endl;
    std::cout << s.__serverConf[0].__autoindex << std::endl;
    std::cout << "-------- server 1 | location 1 --------" << std::endl;
    std::cout << s.__serverConf[0].__location[0].__path << std::endl;
    std::cout << s.__serverConf[0].__location[0].__root << std::endl;
    std::cout << s.__serverConf[0].__location[0].__cgi_ext << std::endl;
    std::cout << "-------- server 1 | location 2 --------" << std::endl;
    std::cout << s.__serverConf[0].__location[1].__path << std::endl;
    std::cout << s.__serverConf[0].__location[1].__root << std::endl;
    std::cout << s.__serverConf[0].__location[1].__cgi_ext << std::endl;
    std::cout << std::endl;
    std::cout << "-------- server 2 --------" << std::endl;
    std::cout << s.__serverConf[1].__port << std::endl;
    std::cout << s.__serverConf[1].__serverName << std::endl;
    std::cout << s.__serverConf[1].__host << std::endl;
    std::cout << s.__serverConf[1].__bodySizeLimit << std::endl;
    std::cout << s.__serverConf[1].__accessLog << std::endl;
    std::cout << s.__serverConf[1].__errorPage << std::endl;
    std::cout << s.__serverConf[1].__autoindex << std::endl;
    std::cout << "-------- server 2 | location 1 --------" << std::endl;
    std::cout << s.__serverConf[1].__location[0].__path << std::endl;
    std::cout << s.__serverConf[1].__location[0].__root << std::endl;
    std::cout << "-------- server 2 | location 2 --------" << std::endl;
    std::cout << s.__serverConf[1].__location[1].__path << std::endl;
    std::cout << s.__serverConf[1].__location[1].__root << std::endl;

    return EXIT_SUCCESS;
} */

/* REQUEST MAIN -- DO NOT TOUCH */
/* int main( int ac, char **av ) {

    (void)ac;
    (void)av;

    Request s = Request();
    s.parseRequest();
    std::cout << std::endl << "------ basic request __dataGatherer -----" << std::endl << std::endl;
    std::cout << s.getDataGatherer() << std::endl;
    std::cout << "------ request line extraction ------" << std::endl << std::endl;
    std::cout << s.getMethod() << std::endl;
    std::cout << s.getUri() << std::endl;
    std::cout << s.getProtocol() << std::endl;
    std::cout << s.getUriExtension() << std::endl;
    std::cout << std::endl << "------ extract headers ------" << std::endl << std::endl;
    std::map<std::string, std::string>::iterator it;
    for (it = s.getHeaders().begin(); it != s.getHeaders().end(); it++) {
        std::cout << it->first << " : " << it->second << std::endl;
    }
    std::cout << std::endl << "------ extract body/content ------" << std::endl << std::endl;
    std::cout << "Body --> " << s.getBodyFilename() << std::endl << std::endl;
    return EXIT_SUCCESS;
} */


/* VECTOR MAIN -- DO NOT TOUCH */
// class m
// {
//     private:
        
//     public:
//         m() {}
//         ~m() {}
// };

// int main( void ) {
//     std::vector<m> __vec;
//     for (int i = 0; i < 4; i++) {
//         __vec.push_back(m());
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