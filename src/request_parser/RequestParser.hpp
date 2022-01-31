/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 10:30:00 by aabounak          #+#    #+#             */
/*   Updated: 2022/01/31 18:25:43 by aabounak         ###   ########.fr       */
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

            /* PVT -- Private method for removing a substring (Made for "\r\r") -- */
            void    __eraseSubstr( std::string & str, const std::string & substr ) {
                __SIZE_TYPE__ pos = str.find(substr);
                if (pos != std::string::npos)
                    str.erase(pos, substr.length());
            }
            void    __eraseAllSubstr( std::string & str, const std::string & substr ) {
                __SIZE_TYPE__ pos = std::string::npos;
                while ((pos = str.find(substr)) != std::string::npos)
                    str.erase(pos, substr.length());
            }

            std::vector<std::string> __split( std::string str, char separator ) {
                std::vector<std::string>  myvec;
                __SIZE_TYPE__ currentIndex = 0, i = 0, startIndex = 0, endIndex = 0;
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
                if (!token.empty() && token[token.size() - 1] == '\r')
                    token.erase(token.size() - 1);
                std::vector<std::string> myvec = __split(token, ' ');
                
                /* --- THROW EXCEPTIONS --- */
                myvec[0] == "GET" or myvec[0] == "POST" or myvec[0] == "DELETE" ? this->__method = myvec[0] : throw UnsupportedMethod();
                this->__path = myvec[1];
                myvec[2] == "HTTP/1.1" ? this->__protocol = myvec[2] : throw UnsupportedTransferProtocol();
            }
            
            /* ----- Getters ----- */


            /* ----- Exceptions ----- */
            class UnsupportedTransferProtocol : public std::exception {
			public:
				virtual const char * what() const throw() {
					return ("UnsupportedTransferProtocol");
				}
		    };

            class UnsupportedMethod : public std::exception {
			public:
				virtual const char * what() const throw() {
					return ("UnsupportedMethod");
				}
		    };
    };
}