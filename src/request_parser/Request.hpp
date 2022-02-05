/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 10:30:00 by aabounak          #+#    #+#             */
/*   Updated: 2022/02/02 19:21:28 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "../TempHeader.hpp"

# define PY 1
# define PHP 2
# define __CONTENT_LENGTH_FOUND__ 0
# define __CONTENT_LENGTH_NOT_FOUND__ 1
# define __BODY_COMPLETE__ 1
# define __BODY_INCOMPLETE__ 2

namespace ft {
    class Request {
        /* ----- PRIVATE ----- */
        public:
            std::string __dataGatherer;
            std::string __method;
            std::string __uri;
            std::string __protocol;
            short       __uriExtension;
            std::map<std::string, std::string>  __headers;
            std::string __bodyFilename;
            
        public:
            /* ----- Constructors & Destructor respectively ----- */
            Request() :
                __dataGatherer(""),
                __method(""),
                __uri(""),
                __protocol(""),
                __uriExtension(0),
                __bodyFilename("") {}
            ~Request() {}

            void    append( std::string x ) {
                __dataGatherer.append(x);
                return ;
            }

            /* PVT -- THIS METHOD SHOULD CHECK FOR STANDARDS LATER -- */
            void    __extractRequestLine( std::istringstream & iss ) {
                std::string line;
                std::getline(iss, line);
                if (!line.empty() && line[line.size() - 1] == '\r')
                    line.erase(line.size() - 1);
                std::vector<std::string> myvec = __split(line, ' ');
                
                myvec[0] == "GET" or myvec[0] == "POST" or myvec[0] == "DELETE" ? this->__method = myvec[0] : throw UnsupportedMethod();
                this->__uri = myvec[1];
                myvec[2] == "HTTP/1.1" ? this->__protocol = myvec[2] : throw UnsupportedTransferProtocol();

                // /* -- SETUP SHORT FOR CGI */
                if (__hasEnding(this->__uri, ".py")) { this->__uriExtension = PY; }
                else if (__hasEnding(this->__uri, ".php")) { this->__uriExtension = PHP; }
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
                /* -- TO COMPLY WITH HTTP/1.1, CLIENTS MUST INCLUDE THE "Host: header" WITH EACH REQUEST -- */
                if (this->__headers.find("host") != this->__headers.end()) { throw HostHeaderUnavailable(); }
            }

            /* PVT -- -- */
            void    __extractContent( std::istringstream & iss ) {
                /* -- WE'RE SURE THAT WE HAVE A POST METHOD */
                if (this->__method == "POST") {
                    std::ofstream f;
                    /* -- NOT A CHUNKED REQUEST -- */
                    if (this->__headers.find("Content-Type") != this->__headers.end()) {
                        if (!(this->__headers.find("Content-Type")->second.empty())) {
                            /* -- ACCORDING TO RFC 2616 SEC4.4 || IN THIS CASE WE SHOULD HAVE A VALID "Content-Length: header" -- */
                            if (__checkContentLength() == __CONTENT_LENGTH_NOT_FOUND__) { throw BadRequest(); }
                            std::string line;
                            this->__bodyFilename = "./src/request_parser/bodyX.txt";
                            f.open(this->__bodyFilename);
                            while (std::getline(iss, line))
                                f << line;
                            /* -- IN THIS CASE IF BODY SIZE AND CL DON'T MATCH A BAD REQUEST SHOULD BE THROW -- */
                            if (__compareContentLengthWithBody(f) != __BODY_COMPLETE__) { f.close(); throw BadRequest(); }
                            f.close(); return ;
                        }
                        else { throw BadRequest(); }
                    }

                    /* -- PARSE CHUNKED REQUESTS */
                    // if (this->__headers.find("Transfer-Encoding") != this->__headers.end()) {
                    //     if (this->__headers.find("Transfer-Encoding")->second == "chunked") {
                    //         std::cout << "I have a chunked request" << std::endl;
                    //     } else {
                    //         std::cout << "Its not chunked" << std::endl;
                    //     }
                    // }

                    // f.close();

                    /* std::string line;
                    const char *s;
                    std::string line;
                    std::getline(iss, line);
                    s = line.c_str();
                    iss.readsome(const_cast<char *>(s), std::stoi(line));
                    std::cout << s << std::endl;
                    std::cout << std::endl; */
                }
            }
            
            /* --- THIS PIECE OF CODE SHOULD BE CHANGED --- */
            void    parseRequest( void ) {
                /* -- THIS IS FOR TEST AND SHOULD BE DELETED LATER */
                std::ifstream   file("./src/request_parser/post_request.example");
                std::string     buffer;
                while (getline(file, buffer)) {
                    this->append(buffer + "\r\n");
                }
                file.close();

                /* -- - */
                std::istringstream  iss(this->__dataGatherer);
                this->__extractRequestLine(iss);
                this->__extractHeaders(iss);
                /* -- CHECK FOR FURTHER STANDARDS */
                if (this->__method == "POST")
                    this->__extractContent(iss);
            }

            
            /* ----- Utils ------ */
            /* -- PVT METHODS */
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
            std::string ltrim( const std::string &s, const std::string &delim ) {
                __SIZE_TYPE__ start = s.find_first_not_of(delim);
                return (start == std::string::npos) ? "" : s.substr(start);
            }
            bool    __hasEnding ( std::string const &fullString, std::string const &ending ) {
                if (fullString.length() >= ending.length()) { return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending)); }
                return false;
            }
            int     __findFileSize( std::ofstream &file ) {
                file.seekp(0, std::ios::end);
                int size = file.tellp();
                return size;
            }

            /* PVT -- -- */
            bool    __checkContentLength( void ) {
                if (this->__headers.find("Content-Length") != this->__headers.end()) { return __CONTENT_LENGTH_FOUND__; }
                return __CONTENT_LENGTH_NOT_FOUND__;
            }

            /* PVT -- -- */
            short   __compareContentLengthWithBody( std::ofstream &f ) {
                if (std::stoi(this->__headers.find("Content-Length")->second) == __findFileSize(f)) { return __BODY_COMPLETE__; }
                return __BODY_INCOMPLETE__;
            }

            /* ----- Getters ----- */


            /* ----- Exceptions ----- */
            class UnsupportedTransferProtocol : public std::exception {
			public:
				virtual const char * what() const throw() {
					return ("Unsupported Transfer Protocol");
				}
		    };
            class UnsupportedMethod : public std::exception {
			public:
				virtual const char * what() const throw() {
					return ("Unsupported Method");
				}
		    };
            class BadRequest : public std::exception {
			public:
				virtual const char * what() const throw() {
					return ("Bad Request");
				}
		    };
            class HostHeaderUnavailable : public std::exception {
			public:
				virtual const char * what() const throw() {
					return ("Host Header Unavailable");
				}
		    };
    };
}