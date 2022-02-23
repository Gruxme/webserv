/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:45:08 by aabounak          #+#    #+#             */
/*   Updated: 2022/02/23 11:31:36 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Request.hpp"

/* ----- Constructors & Destructor respectively ----- */
Request::Request() :
    _dataGatherer(""),
    _method(""),
    _uri(""),
    _protocol(""),
    _uriExtension(0),
	_port(8080),
    _bodyFilename(""),
	_status(false) {}

Request::~Request() {}

Request::Request( Request const &x ) { *this = x; }

Request& Request::operator=( Request const &rhs ) {
    if (this != &rhs) {
        this->_dataGatherer = rhs._dataGatherer;
        this->_method = rhs._method;
        this->_uri = rhs._uri;
        this->_protocol = rhs._protocol;
        this->_uriExtension = rhs._uriExtension;
        this->_headers = rhs._headers;
        this->_port = rhs._port;
        this->_bodyFilename = rhs._bodyFilename;
        this->_status = rhs._status;
    }
    return *this;
}

/* ----- Getters ----- */

std::string Request::getMethod( void ) const { return this->_method; }
std::string Request::getUri( void ) const { return this->_uri; }
std::string Request::getProtocol(void ) const { return this->_protocol; }
short       Request::getUriExtension( void ) const { return this->_uriExtension; }
std::string Request::getBodyFilename( void ) const { return this->_bodyFilename; }
bool		Request::isComplete( void ) const { return _status; }
std::map<std::string, std::string> const& Request::getHeaders( void ) const { return this->_headers; }
int 		Request::getPort( void ) const { return this->_port; }
/* TO BE DELETED */ std::string Request::getDataGatherer( void ) const { return this->_dataGatherer; }

/* -- PUBLIC METHODS */
void    Request::append( const char * recvBuffer ) {
    std::string x(recvBuffer);
    _dataGatherer.append(x);
    return ;
}

bool	Request::_headersComplete( void ) {
	return _dataGatherer.find("\r\n\r\n") != std::string::npos;
}

bool    Request::_bodyComplete( void ) {
    return _dataGatherer.find("0\r\n\r\n") != std::string::npos;
}

void	Request::parse( void ) {
	// int	contentLength = 0;
	if (_headersComplete() == true) {
		if (_headers.empty() == true) {
			std::stringstream	iss(_dataGatherer);
			_extractRequestLine(iss);
			_extractHeaders(iss);
            if (this->_method == "POST") {
                std::map<std::string, std::string>::iterator transferEncoding = _headers.find("Transfer-Encoding");
                if (transferEncoding != _headers.end() && transferEncoding->second == "chunked") {
                    if (_bodyComplete() == true) {
                        /* -- CHECK THAT WE DONT HAVE A DUPLICATE FILE */
                        _handleChunkedRequest(iss);
						_status = true;
                    }
                }
                /* ------
                    CAN BE CHANGED
                    {{ THIS WOULD EITHER GET COMPLETED AND isComplete will return TRUE
                    OR AN EXCEPTION WILL BE THROWN
                ------ */
                _handleBasicRequest(iss);
				_status = true;
            }
            else{
				_status = true;
			}
		}
	}
    _status  = false;
}

/* -- PVT PARSE METHODS */
void    Request::_extractRequestLine( std::stringstream & iss ) {
    std::string line;
    std::getline(iss, line);
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    std::vector<std::string> myvec = _split(line, ' ');
    myvec[0] == "GET" or myvec[0] == "POST" or myvec[0] == "DELETE" ? this->_method = myvec[0] : throw parseErr("405 Method Not Allowed"); //generate a Allow header in response
    this->_uri = myvec[1];
    myvec[2] == "HTTP/1.1" ? this->_protocol = myvec[2] : throw parseErr("505 HTTP Version Not Supported");
    // /* -- SETUP SHORT FOR CGI */
    if (_hasEnding(this->_uri, ".py")) { this->_uriExtension = PY; }
    else if (_hasEnding(this->_uri, ".php")) { this->_uriExtension = PHP; }
}

void    Request::_extractHeaders( std::stringstream & iss ) {
    std::string line;
    std::vector<std::string> myvec(0);
    /* -- DO ERROR TREATMENTS ON STANDARDS IN HERE */
    while (std::getline(iss, line)) {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        if (line.size() == 0)
            break ;
        myvec = _split(line, ':');
        if (myvec.at(0).empty() || myvec.at(1).empty())
            throw parseErr("400 Bad Request");
        myvec[1] = this->_ltrim(myvec[1], " ");
        this->_headers[myvec[0]] = myvec[1];
		if (myvec[0] == "host") { this->_port = std::stoi(_split(myvec[1], ':')[1]); }
    }
    /* ------
        TO COMPLY WITH HTTP/1.1, CLIENTS MUST INCLUDE THE "Host: header" WITH EACH REQUEST
    ------ */
    if (this->_headers.find("host") != this->_headers.end()) { throw parseErr("400 Bad Request"); } // ==?
}

void Request::_handleChunkedRequest( std::stringstream & iss ) {
    /* ------
        "Messages MUST NOT include both a Content-Length header field and a non-identity transfer-coding.
        If the message does include a non-identity transfer-coding, the Content-Length MUST be ignored."
        (RFC 2616, Section 4.4)
    ------ */
    std::ofstream f;
    std::string line;
    uint16_t n = 0;
    this->_bodyFilename = "./src/request/bodyChunked.txt";
    f.open(this->_bodyFilename);
    while (std::getline(iss, line)) {
        line.erase(line.find_last_of('\r'));
        if (_isHexNotation(line))
            n = _hexadecimalToDecimal(line);
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
}

void    Request::_handleBasicRequest( std::stringstream & iss ) {
    /* ------
        IN THIS CASE WE SHOULD HAVE A VALID "Content-Length: header"
        ACCORDING TO RFC 2616 SEC4.4
    ------ */
    if (_checkContentLength() == _CONTENT_LENGTH_NOT_FOUND_ ||
            _checkContentLength() == _CONTENT_LENGTH_NEGATIVE_)
        throw parseErr("400 Bad Request");
    std::ofstream f;
    std::string line;
    this->_bodyFilename = "./src/request/bodyX.txt";
    f.open(this->_bodyFilename);
    f << iss.rdbuf();
    /* ------
        IF BODY SIZE AND CONTENT-LENGTH DON'T MATCH A BAD REQUEST SHOULD BE THROW
    ------ */
    if (_compareContentLengthWithBody(f) != _BODY_COMPLETE_) {
		//test if precedency works only
        f.close();
        unlink("/src/request/bodyX.txt");
        throw parseErr("400 Bad Request");
    }
    f.close();
}

/* ----- Utils ------ */
/* -- PVT METHODS */

std::vector<std::string> Request::_split( std::string str, char separator ) {
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

void    Request::_eraseSubstr( std::string &str, const std::string &substr ) {
    size_t pos = str.find(substr);
    if (pos != std::string::npos)
        str.erase(pos, substr.length());
}

void    Request::_eraseAllSubstr( std::string &str, const std::string &substr ) {
    size_t pos = std::string::npos;
    while ((pos = str.find(substr)) != std::string::npos)
        str.erase(pos, substr.length());
}

std::string Request::_ltrim( const std::string &s, const std::string &delim ) {
    size_t start = s.find_first_not_of(delim);
    return (start == std::string::npos) ? "" : s.substr(start);
}

bool    Request::_hasEnding( std::string const &fullString, std::string const &ending ) {
    if (fullString.length() >= ending.length()) { return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending)); }
    return false;
}

int     Request::_findFileSize( std::ofstream &file ) {
    file.seekp(0, std::ios::end);
    int size = file.tellp();
    return size;
}

bool    Request::_isHexNotation( std::string const& s ) {
    return s.find_first_not_of("0123456789abcdefABCDEF\r\n", 2) == std::string::npos;
}

int     Request::_hexadecimalToDecimal( std::string hexVal ) {
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

bool    Request::_checkContentLength( void ) {
    if (this->_headers.find("Content-Length") != this->_headers.end()) {
        int contentLength = 0;
        try {
            contentLength = std::stoi(this->_headers.find("Content-Length")->second);
            return (contentLength >= 0 ? _CONTENT_LENGTH_FOUND_ : _CONTENT_LENGTH_NEGATIVE_);
        } catch ( std::exception const &e ) {
            std::cout << e.what() << std::endl;
        }
    }
    return _CONTENT_LENGTH_NOT_FOUND_;
}

short   Request::_compareContentLengthWithBody( std::ofstream &f ) {
    if (std::stoi(this->_headers.find("Content-Length")->second) == _findFileSize(f)) { return _BODY_COMPLETE_; }
    return _BODY_INCOMPLETE_;
}


/* -- OPERATOR << */
std::ostream & operator<<( std::ostream & o, Request const & req ) {
	o << req.getMethod() + " ";
	o << req.getUri() << " ";
	o << req.getProtocol() << " \n";
    std::map<std::string, std::string>::const_iterator it = req.getHeaders().begin();
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
