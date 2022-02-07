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
        private:
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

            std::string getMethod( void ) const { return this->__method; }
            std::string getUri( void ) const { return this->__uri; }
            std::string getProtocol(void ) const { return this->__protocol; }
            short       getUriExtension( void ) const { return this->__uriExtension; }
            std::map<std::string, std::string>  getHeaders( void ) const { return this->__headers; }
            std::string getBodyFilename( void ) const { return this->__bodyFilename; }

            void    append( char * recvBuffer ) {
                std::string x(recvBuffer);
                x.erase(std::remove(x.begin(), x.end(), '\r'), x.end());
                __dataGatherer.append(x + "\n");
                return ;    
            }

        private:
            /* PVT -- THIS METHOD SHOULD CHECK FOR STANDARDS LATER -- */
            void    __extractRequestLine( std::istringstream & iss ) {
                std::string line;
                std::getline(iss, line);
                std::vector<std::string> myvec = __split(line, ' ');
                myvec[0] == "GET" or myvec[0] == "POST" or myvec[0] == "DELETE" ? this->__method = myvec[0] : throw parseErr("Unsupported Method");
                this->__uri = myvec[1];
                myvec[2] == "HTTP/1.1" ? this->__protocol = myvec[2] : throw parseErr("Unsupported Transfer Protocol");
                // /* -- SETUP SHORT FOR CGI */
                if (__hasEnding(this->__uri, ".py")) { this->__uriExtension = PY; }
                else if (__hasEnding(this->__uri, ".php")) { this->__uriExtension = PHP; }
            }

            /* PVT -- -- */
            void    __extractHeaders( std::istringstream & iss ) {
                std::string line;
                std::vector<std::string> myvec;

                /* -- DO ERROR TREATMENTS ON STANDARDS IN HERE */
                while (std::getline(iss, line)) {
                    if (line.size() == 0)
                        break ;
                    myvec = __split(line, ':');
                    if (myvec[0].empty() || myvec[1].empty())
                        throw parseErr("Bad Request");
                    myvec[1] = this->__ltrim(myvec[1], " ");
                    this->__headers[myvec[0]] = myvec[1];
                }
                /* -- TO COMPLY WITH HTTP/1.1, CLIENTS MUST INCLUDE THE "Host: header" WITH EACH REQUEST -- */
                if (this->__headers.find("host") != this->__headers.end()) { throw parseErr("Host Header Unavailable"); }
            }

            /* PVT -- -- */
            void    __extractContent( std::istringstream & iss ) {
                /* -- WE'RE SURE THAT WE HAVE A POST METHOD */
                if (this->__method == "POST") {
                    std::ofstream f;
                    if (this->__headers.find("Transfer-Encoding") != this->__headers.end()) {
                        /* -- CHUNKED REQUEST */
                        if (this->__headers.find("Transfer-Encoding")->second == "chunked") {
                            std::string line;
                            this->__bodyFilename = "./src/request_parser/bodyChunked.txt";
                            f.open(this->__bodyFilename);
                            uint16_t    n = 0;
                            while (std::getline(iss, line)) {
                                if (__isHexNotation(line)) {
                                    if (__isHexNotation(line)) n = __hexadecimalToDecimal(line);
                                    else n = stoi(line);
                                }
                                else {
                                    if (n > line.length()) {
                                        int x = line.length();
                                        line += '\n';
                                        while (x < n) {
                                            std::string buffer;
                                            std::getline(iss, buffer);
                                            x += buffer.length() + 2;
                                            line += buffer + "\n";
                                        }
                                        line.erase(line.end() - 1);
                                        f << line;
                                    }
                                    else
                                        f << line;
                                }  
                            }
                            f.close();
                            return ;
                        }
                    }

                    /* -- REQUEST IS NOT CHUNKED -- */
                    /* -- ACCORDING TO RFC 2616 SEC4.4 || IN THIS CASE WE SHOULD HAVE A VALID "Content-Length: header" -- */
                    if (__checkContentLength() == __CONTENT_LENGTH_NOT_FOUND__) {
                        throw parseErr("Bad Request");
                    }
                    std::string line;
                    this->__bodyFilename = "./src/request_parser/bodyX.txt";
                    f.open(this->__bodyFilename);
                    while (std::getline(iss, line))
                        f << line;
                    /* -- IN THIS CASE IF BODY SIZE AND CONTENT-LENGTH DON'T MATCH A BAD REQUEST SHOULD BE THROW -- */
                    if (__compareContentLengthWithBody(f) != __BODY_COMPLETE__) {
                        f.close();
                        throw parseErr("Bad Request");
                    }
                    f.close();
                    return ;
                }
            }

        public:
            /* --- THIS PIECE OF CODE SHOULD BE CHANGED --- */
            void    parseRequest( void ) {

                std::string s = "POST /cgi-bin/process.cgi HTTP/1.1\n\
User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\n\
Host: www.google.com\n\
Content-Type: application/x-www-form-urlencoded\n\
Content-Length: 205\n\
Accept-Language: en-us\n\
Accept-Encoding: gzip, deflate\n\
Connection: Keep-Alive\n\
Transfer-Encoding: chunked\n\n\
4\r\n\
Wiki\r\n\
6\r\n\
pedia \r\n\
0xE\r\n\
in \r\n\
\r\n\
chunks.\r\n\
0\r\n\
\r\n";

                std::istringstream ss(s);
                std::string buffer;
                while (getline(ss, buffer)) {
                    this->append(buffer);
                }

                /* -- - */
                std::istringstream  iss(this->__dataGatherer);
                this->__extractRequestLine(iss);
                this->__extractHeaders(iss);
                /* -- CHECK FOR FURTHER STANDARDS */
                // if (this->__method == "POST")
                //     this->__extractContent(iss);
            }

            
            /* ----- Utils ------ */
        private:
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
            std::string __ltrim( const std::string &s, const std::string &delim ) {
                __SIZE_TYPE__ start = s.find_first_not_of(delim);
                return (start == std::string::npos) ? "" : s.substr(start);
            }
            bool    __hasEnding( std::string const &fullString, std::string const &ending ) {
                if (fullString.length() >= ending.length()) { return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending)); }
                return false;
            }
            int     __findFileSize( std::ofstream &file ) {
                file.seekp(0, std::ios::end);
                int size = file.tellp();
                return size;
            }
            bool    __isHexNotation( std::string const& s ) {
                return s.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
            }
            int     __hexadecimalToDecimal(std::string hexVal)
            {
                int len = hexVal.size();
                int base = 1;
                int dec_val = 0;
                for (int i = len - 1; i >= 0; i--) {
                    if (hexVal[i] >= '0' && hexVal[i] <= '9') {
                        dec_val += (int(hexVal[i]) - 48) * base;
                        base = base * 16;
                    }
                    else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {
                        dec_val += (int(hexVal[i]) - 55) * base;
                        base = base * 16;
                    }
                }
                return dec_val;
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
			class parseErr : public std::exception {
			public:
				explicit parseErr(const std::string &errStr) throw() : _errStr(errStr) {}
				virtual const char * what() const throw() {
					return (_errStr.c_str());
				}
			private:
				std::string _errStr;
		    };

    };
}
