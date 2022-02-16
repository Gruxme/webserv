/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:45:08 by aabounak          #+#    #+#             */
/*   Updated: 2022/02/16 10:41:51 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Request.hpp"

/* ----- Constructors & Destructor respectively ----- */
Request::Request() :
    __dataGatherer(""),
    __method(""),
    __uri(""),
    __protocol(""),
    __uriExtension(0),
    __bodyFilename("") {}

Request::~Request() {}


/* ----- Getters ----- */

/* TO BE DELETED */ std::string Request::getDataGatherer( void ) const { return this->__dataGatherer; }
std::string Request::getMethod( void ) const { return this->__method; }
std::string Request::getUri( void ) const { return this->__uri; }
std::string Request::getProtocol(void ) const { return this->__protocol; }
short       Request::getUriExtension( void ) const { return this->__uriExtension; }
std::map<std::string, std::string> const& Request::getHeaders( void ) const { return this->__headers; }
std::string Request::getBodyFilename( void ) const { return this->__bodyFilename; }



/* -- PUBLIC METHODS */
void    Request::append( const char * recvBuffer ) {
    std::string x(recvBuffer);
    // x.erase(std::remove(x.begin(), x.end(), '\r'), x.end());
    __dataGatherer.append(x);
    return ;
}

bool	Request::headersComplete( void ){
	return __dataGatherer.find("\r\n\r\n") != std::string::npos;
}

// bool	Request::isComplete( void ){
// 	int	contentLenght = 0;
// 	if(headersComplete()){
// 		if(__headers.empty()){
// 			std::istringstream	iss(__dataGatherer);
// 			__extractRequestLine(iss);
// 			__extractHeaders(iss);
// 		}
// 		std::map<std::string, std::string>::iterator transferEncoding = __headers.find("Transfer-Encoding");
// 		if(transferEncoding != __headers.end() && transferEncoding->second == "chunked")
// 			//look for 0\r\n\r\n if found data is then complete
// 		contentLenght = std::stoi(__headers.find("Content-Length")->second);
// 		//check if content length can be neg and throw exception if it is
// 		//when contentLenght == length of request body, then req is complete

// 	}
// }

/* -- PVT PARSE METHODS */
void    Request::__extractRequestLine( std::stringstream & iss ) {
    std::string line;
    std::getline(iss, line);
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    std::vector<std::string> myvec = __split(line, ' ');
    myvec[0] == "GET" or myvec[0] == "POST" or myvec[0] == "DELETE" ? this->__method = myvec[0] : throw parseErr("Unsupported Method");
    this->__uri = myvec[1];
    myvec[2] == "HTTP/1.1" ? this->__protocol = myvec[2] : throw parseErr("Unsupported Transfer Protocol");
    // /* -- SETUP SHORT FOR CGI */
    if (__hasEnding(this->__uri, ".py")) { this->__uriExtension = PY; }
    else if (__hasEnding(this->__uri, ".php")) { this->__uriExtension = PHP; }
}

void    Request::__extractHeaders( std::stringstream & iss ) {
    std::string line;
    std::vector<std::string> myvec(0);

    /* -- DO ERROR TREATMENTS ON STANDARDS IN HERE */
    while (std::getline(iss, line)) {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        if (line.size() == 0)
            break ;
        myvec = __split(line, ':');
        if (myvec.at(0).empty() || myvec.at(1).empty())
            throw parseErr("Bad Request");
        myvec[1] = this->__ltrim(myvec[1], " ");
        this->__headers[myvec[0]] = myvec[1];
    }
    /* -- TO COMPLY WITH HTTP/1.1, CLIENTS MUST INCLUDE THE "Host: header" WITH EACH REQUEST -- */
    if (this->__headers.find("host") != this->__headers.end()) { throw parseErr("Host Header Unavailable"); }
}

void    Request::__extractContent( std::stringstream & iss ) {
    /* -- WE'RE SURE THAT WE HAVE A POST METHOD */
    if (this->__method == "POST") {
        std::ofstream f;
        if (this->__headers.find("Transfer-Encoding") != this->__headers.end()) {
            /* -- CHUNKED REQUEST */
            if (this->__headers.find("Transfer-Encoding")->second == "chunked") {
                std::string line;
                std::string kharya;
                this->__bodyFilename = "./src/request/bodyChunked.txt";
                f.open(this->__bodyFilename);
                uint16_t    n = 0;
                while (std::getline(iss, line)) {
                    line.erase(line.find_last_of('\r'));
                    if (__isHexNotation(line))
                        n = __hexadecimalToDecimal(line);
                    else {
                        if (n > line.length()) {
                            int x = line.length();
                            line += "\n";
                            while (x < (n)) {
                                std::string buffer;
                                std::getline(iss, buffer);
                                buffer.erase(buffer.find_last_of('\r'));
                                x += buffer.length() + 2;
                                line += buffer + "\n";
                            }
                            line.erase(line.find_last_of('\n'));
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
        this->__bodyFilename = "./src/request/bodyX.txt";
        f.open(this->__bodyFilename);
        f << iss.rdbuf();
            
        /* -- IN THIS CASE IF BODY SIZE AND CONTENT-LENGTH DON'T MATCH A BAD REQUEST SHOULD BE THROW -- */
        if (__compareContentLengthWithBody(f) != __BODY_COMPLETE__) {
            f.close();
            unlink("/src/request/bodyX.txt");
            throw parseErr("Bad Request");
        }
        f.close();
        return ;
    }
}

void    Request::parseRequest( void ) {
    /* -- - */
    std::stringstream  iss(this->__dataGatherer);
    this->__extractRequestLine(iss);
    this->__extractHeaders(iss);
    // /* -- CHECK FOR FURTHER STANDARDS */
    if (this->__method == "POST")
        this->__extractContent(iss);
}

/* ----- Utils ------ */
/* -- PVT METHODS */

std::vector<std::string> Request::__split( std::string str, char separator ) {
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

void    Request::__eraseSubstr( std::string &str, const std::string &substr ) {
    __SIZE_TYPE__ pos = str.find(substr);
    if (pos != std::string::npos)
        str.erase(pos, substr.length());
}

void    Request::__eraseAllSubstr( std::string &str, const std::string &substr ) {
    __SIZE_TYPE__ pos = std::string::npos;
    while ((pos = str.find(substr)) != std::string::npos)
        str.erase(pos, substr.length());
}

std::string Request::__ltrim( const std::string &s, const std::string &delim ) {
    __SIZE_TYPE__ start = s.find_first_not_of(delim);
    return (start == std::string::npos) ? "" : s.substr(start);
}

bool    Request::__hasEnding( std::string const &fullString, std::string const &ending ) {
    if (fullString.length() >= ending.length()) { return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending)); }
    return false;
}

int     Request::__findFileSize( std::ofstream &file ) {
    file.seekp(0, std::ios::end);
    int size = file.tellp();
    return size;
}

bool    Request::__isHexNotation( std::string const& s ) {
    return s.find_first_not_of("0123456789abcdefABCDEF\r\n", 2) == std::string::npos;
}

int     Request::__hexadecimalToDecimal( std::string hexVal ) {
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

bool    Request::__checkContentLength( void ) {
    if (this->__headers.find("Content-Length") != this->__headers.end()) { return __CONTENT_LENGTH_FOUND__; }
    return __CONTENT_LENGTH_NOT_FOUND__;
}

short   Request::__compareContentLengthWithBody( std::ofstream &f ) {
    if (std::stoi(this->__headers.find("Content-Length")->second) == __findFileSize(f)) { return __BODY_COMPLETE__; }
    return __BODY_INCOMPLETE__;
}


/* -- OPERATOR << */
/* std::ostream & operator<<( std::ostream & o, Request const & req ) {
	o << req.getMethod() + " ";
	o << req.getUri() << " ";
	o << req.getProtocol() << " \n";
    std::map<std::string, std::string>::iterator it = req.getHeaders().begin();
	for (; it != req.getHeaders().end(); it++){
		o << it->first << ": " << it->second << std::endl;
	}
	std::cout << std::endl;
	std::ifstream	body(req.getBodyFilename());
	std::string line;
	while(getline(body, line))
		std::cout << line << std::endl;
	return o;
}
 */