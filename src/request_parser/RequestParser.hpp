/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 10:30:00 by aabounak          #+#    #+#             */
/*   Updated: 2022/01/31 16:35:55 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../TempHeader.hpp"

namespace ft {
    class RequestParser {
        /* ----- PRIVATE ----- */
        public:
            std::string __content;

            std::string __method;
            std::string __path;
            std::string __protocol;
            
            std::map<std::string, std::string>  __headers;
            
        public:
            /* ----- Constructors & Destructor respectively ----- */
            RequestParser() : __content(""), __method(""), __path(""), __protocol("") {}
            ~RequestParser() {}

            void    append( std::string x ) {
                __content.append(x);
                return ;
            }

            /* --- This piece of code should be changed --- */
            void    parseRequest( void ) {
                std::ifstream   file("./src/request_parser/post_request.example");
                std::string     buffer;
                while (getline(file, buffer)) {
                    // \r\n is temporary
                    this->append(buffer + "\r\n");
                }
            }


            /* --- This method splits a string and returns a vector of strings --- */
            std::vector<std::string> __split( std::string str, char separator ) {
                std::vector<std::string>  myvec;
                size_t currentIndex = 0, i = 0, startIndex = 0, endIndex = 0;
                while (i <= str.length()) {
                    if (str[i] == separator || i == str.length()) {
                        endIndex = i;
                        std::string subString = "";
                        subString.append(str, startIndex, endIndex - startIndex);
                        myvec.push_back(subString);
                        currentIndex++;
                        startIndex = endIndex + 1;
                    }
                    i++;
                }
                return myvec;
            }

            /* PVT -- THIS METHOD SHOULD CHECK FOR STANDARDS LATER -- */
            void    __extractRequestLine( void ) {
                std::istringstream iss(this->__content);
                std::string token;
                std::getline(iss, token);
                std::vector<std::string> myvec = __split(token, ' ');
                /* --- THROW EXCEPTIONS --- */
                myvec[2].erase(myvec[2].size() - 1);
                myvec[0] == "GET" or myvec[0] == "POST" or myvec[0] == "DELETE" ? this->__method = myvec[0] : throw "Server doesn't handle this method";
                myvec[2] == "HTTP/1.1" ? this->__protocol = myvec[2] : throw "Server doesn't handle this TP";
                this->__path = myvec[1];
            }
            
            /* ----- Getters ----- */
    };
}