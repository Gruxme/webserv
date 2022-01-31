/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 11:26:34 by aabounak          #+#    #+#             */
/*   Updated: 2022/01/31 17:44:08 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "src/TempHeader.hpp"

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


/* CONFIGURATION FILE MAIN -- DO NOT TOUCH */
/* int main( int ac, char **av ) {

    (void)ac;
    (void)av;

    ft::ConfigFileParser s = ft::ConfigFileParser();
    s.parseConfigFile();
    std::cout << "-------- server 1 --------" << std::endl;
    std::cout << s.__serverConf[0].__port << std::endl;
    std::cout << s.__serverConf[0].__serverName << std::endl;
    std::cout << s.__serverConf[0].__host << std::endl;
    std::cout << s.__serverConf[0].__bodySizeLimit << std::endl;
    std::cout << s.__serverConf[0].__accessLog << std::endl;
    std::cout << s.__serverConf[0].__errorPage << std::endl;
    std::cout << "-------- server 1 | location 1 --------" << std::endl;
    std::cout << s.__serverConf[0].__location[0].__path << std::endl;
    std::cout << s.__serverConf[0].__location[0].__root << std::endl;
    std::cout << "-------- server 1 | location 2 --------" << std::endl;
    std::cout << s.__serverConf[0].__location[1].__path << std::endl;
    std::cout << s.__serverConf[0].__location[1].__root << std::endl;
    std::cout << std::endl;
    std::cout << "-------- server 2 --------" << std::endl;
    std::cout << s.__serverConf[1].__port << std::endl;
    std::cout << s.__serverConf[1].__serverName << std::endl;
    std::cout << s.__serverConf[1].__host << std::endl;
    std::cout << s.__serverConf[1].__bodySizeLimit << std::endl;
    std::cout << s.__serverConf[1].__accessLog << std::endl;
    std::cout << s.__serverConf[1].__errorPage << std::endl;
    std::cout << "-------- server 2 | location 1 --------" << std::endl;
    std::cout << s.__serverConf[1].__location[0].__path << std::endl;
    std::cout << s.__serverConf[1].__location[0].__root << std::endl;
    std::cout << "-------- server 2 | location 2 --------" << std::endl;
    std::cout << s.__serverConf[1].__location[1].__path << std::endl;
    std::cout << s.__serverConf[1].__location[1].__root << std::endl;

    return EXIT_SUCCESS;
} */

/* REQUEST MAIN -- DO NOT TOUCH */
int main( int ac, char **av ) {

    (void)ac;
    (void)av;

    ft::RequestParser s = ft::RequestParser();
    s.parseRequest();
    
    std::cout << "------ basic request __content -----" << std::endl << std::endl;
    std::cout << s.__content << std::endl;
    std::cout << "------ request line extraction ------" << std::endl << std::endl;
    s.__extractRequestLine();
    std::cout << s.__method << std::endl;
    std::cout << s.__path << std::endl;
    std::cout << s.__protocol << std::endl;
    return EXIT_SUCCESS;
}