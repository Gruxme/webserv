/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 11:26:34 by aabounak          #+#    #+#             */
/*   Updated: 2022/01/20 22:18:34 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "header.hpp"

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
int main( int ac, char **av ) {
    ft::ConfigFileParser s = ft::ConfigFileParser();
    s.parseConfigFile();
    std::cout << "-------- server 1 --------" << std::endl;
    std::cout << s.__serverConf[0].__port << std::endl;
    std::cout << s.__serverConf[0].__serverName << std::endl;
    std::cout << s.__serverConf[0].__host << std::endl;
    std::cout << "-------- server 1 | location 1 --------" << std::endl;
    std::cout << s.__serverConf[0].__location[0].__path << std::endl;
    std::cout << s.__serverConf[0].__location[0].__root << std::endl;
    std::cout << s.__serverConf[0].__location[0].__access_log << std::endl;
    std::cout << "-------- server 1 | location 2 --------" << std::endl;
    std::cout << s.__serverConf[0].__location[1].__path << std::endl;
    std::cout << s.__serverConf[0].__location[1].__root << std::endl;
    std::cout << s.__serverConf[0].__location[1].__access_log << std::endl;
    std::cout << std::endl;
    std::cout << "-------- server 2 --------" << std::endl;
    std::cout << s.__serverConf[1].__port << std::endl;
    std::cout << s.__serverConf[1].__serverName << std::endl;
    std::cout << s.__serverConf[1].__host << std::endl;
    std::cout << "-------- server 2 | location 1 --------" << std::endl;
    std::cout << s.__serverConf[1].__location[0].__path << std::endl;
    std::cout << s.__serverConf[1].__location[0].__root << std::endl;
    std::cout << s.__serverConf[1].__location[0].__access_log << std::endl;
    std::cout << "-------- server 2 | location 2 --------" << std::endl;
    std::cout << s.__serverConf[1].__location[1].__path << std::endl;
    std::cout << s.__serverConf[1].__location[1].__root << std::endl;
    std::cout << s.__serverConf[1].__location[1].__access_log << std::endl;

    return EXIT_SUCCESS;
}