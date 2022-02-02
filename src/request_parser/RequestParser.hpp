/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 10:30:00 by aabounak          #+#    #+#             */
/*   Updated: 2022/02/02 18:30:40 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../TempHeader.hpp"

namespace ft {
    class RequestParser {
        /* ----- PRIVATE ----- */
        public:
            std::string __content;

            std::string __method;
            std::string __uri;
            std::string __protocol;

            std::string __body;
            
            std::map<std::string, std::string>  __headers;
            
        public:
            /* ----- Constructors & Destructor respectively ----- */
            RequestParser() : __content(""), __method(""), __uri(""), __protocol(""), __body("") {}
            ~RequestParser() {}

            void    append( std::string x ) {
                __content.append(x);
                return ;
            }

            /* PVT -- THIS METHOD SHOULD CHECK FOR STANDARDS LATER -- */
            void    __extractRequestLine( std::istringstream & iss ) {
                std::string line;
                std::getline(iss, line);
                if (!line.empty() && line[line.size() - 1] == '\r')
                    line.erase(line.size() - 1);
                std::vector<std::string> myvec = __split(line, ' ');
                
                /* --- THROW EXCEPTIONS --- */
                myvec[0] == "GET" or myvec[0] == "POST" or myvec[0] == "DELETE" ? this->__method = myvec[0] : throw UnsupportedMethod();
                this->__uri = myvec[1];
                myvec[2] == "HTTP/1.1" ? this->__protocol = myvec[2] : throw UnsupportedTransferProtocol();
            }

            /* PVT -- -- */
            void    __extractHeaders( std::istringstream & iss ) {
                std::string line;
                std::vector<std::string> myvec;
                while (std::getline(iss, line)) {
                    if (line.size() == 1)
                        break ;
                    if (!line.empty() && line[line.size() - 1] == '\r')
                        line.erase(line.size() - 1);
                    myvec = __split(line, ':');
                    myvec[1] = this->ltrim(myvec[1], " :");
                    this->__headers[myvec[0]] = myvec[1];
                }
            }

            /* PVT -- -- */
            void    __extractContent( std::istringstream & iss ) {

                /* -- THIS IS FOR RAW DATA */
                // std::string line;
                // std::cout << std::endl;
                // while (std::getline(iss, line)) {
                //     /* -- TO RECALL LATER */
                //     this->__body.append(line + "\r\n");
                // }

                /* -- PARSE CHUNKED REQUESTS */
                std::string line;
                std::cout << std::endl;
                
                while (std::getline(iss, line)) {
                    const char *x = line.c_str(); ++x;
                    if (std::isdigit(line[0]) && strcmp(x, "\r\n"))
                        std::getline(iss, line);
                    try { 
                        if (std::stoi(line, 0, 16)) {
                            
                        }
                    } catch ()
                    std::cout << line << std::endl;
                }

                /*
                    Wikipedia in 
    
                    chunks.
                */
                std::cout << std::endl;
            }
            
            /* --- THIS PIECE OF CODE SHOULD BE CHANGED --- */
            void    parseRequest( void ) {

                /* -- THIS IS FOR TEST AND SHOULD BE DELETED LATER */
                std::ifstream   file("./src/request_parser/post_request.example");
                std::string     buffer;
                while (getline(file, buffer)) {
                    this->append(buffer + "\r\n");
                }

                /* -- REFACTOR {PHASE 1} */
                std::istringstream  iss(this->__content);
                this->__extractRequestLine(iss);
                this->__extractHeaders(iss);
                this->__extractContent(iss);
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
            
            /* ----- Utils ------ */
            /* PVT -- Private methods for removing a substring (Made for "\r\n") -- */
            void    __eraseSubstr( std::string &str, const std::string &substr ) {
                __SIZE_TYPE__ pos = str.find(substr);
                if (pos != std::string::npos)
                    str.erase(pos, substr.length());
            }
            void    __eraseAllSubstr( std::string &str, const std::string &substr ) {
                __SIZE_TYPE__ pos = std::string::npos;
                while ((pos = str.find(substr)) != std::string::npos)
                    str.erase(pos, substr.length());
            }

            std::string ltrim( const std::string &s, const std::string &delim )
            {
                size_t start = s.find_first_not_of(delim);
                return (start == std::string::npos) ? "" : s.substr(start);
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