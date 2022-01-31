/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 10:30:00 by aabounak          #+#    #+#             */
/*   Updated: 2022/01/31 11:08:23 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../TempHeader.hpp"

namespace ft {
    class RequestParser {
        /* ----- PRIVATE ----- */
        public:
            std::string __content;
            
        public:
            /* ----- Constructors & Destructor respectively ----- */
            RequestParser() {}
            ~RequestParser() {}

            void    append( std::string x ) {
                __content.append(x);
                return ;
            }

            void    parseRequest( void ) {
                std::string ex = "test\r\n";
                std::string body = "all the html\r\n";
                for (int i = 0; i < 4; i++) {
                    append(ex);
                }
                append("\r\n");
                append(body);
                return ;
            }
            
            /* ----- Getters ----- */
    };
}