/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-fadi <sel-fadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:45:08 by aabounak          #+#    #+#             */
/*   Updated: 2022/03/11 13:18:26 by sel-fadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Request.hpp"

/* ----- Constructors & Destructor respectively ----- */
Request::Request() :
    _dataGatherer(),
    _method(""),
    _uri(""),
    _query(""),
    _path(""),
    _protocol(""),
    _uriExtension(0),
	_port(0), // to be filled with default port of default server
    _bodyFilename(""),
	_status(false),
    _config(),
    _fileName(""),
    _pos(-1),
    _bodyFd(-1),
    _totalBytesRead(0),
    _headersPassed(false) {}

Request::~Request() {
    close(this->_bodyFd);
}

Request::Request( Request const &x ) { *this = x; }

Request& Request::operator=( Request const &rhs ) {
    if (this != &rhs) {
        this->_dataGatherer = rhs._dataGatherer;
        this->_method = rhs._method;
        this->_uri = rhs._uri;
        this->_query = rhs._query;
        this->_path = rhs._path;
        this->_protocol = rhs._protocol;
        this->_uriExtension = rhs._uriExtension;
        this->_headers = rhs._headers;
        this->_port = rhs._port;
        this->_bodyFilename = rhs._bodyFilename;
        this->_status = rhs._status;
        this->_config = rhs._config;
        this->_fileName = rhs._fileName;
        this->_pos = rhs._pos;
        this->_bodyFd = rhs._bodyFd;
        this->_totalBytesRead = rhs._totalBytesRead;
    }
    return *this;
}

/* ----- Getters ----- */

/* TO BE DELETED */ std::string Request::getDataGatherer( void ) const { return this->_dataGatherer; }
std::string Request::getMethod( void ) const { return this->_method; }
std::string Request::getUri( void ) const { return this->_uri; }
std::string Request::getQuery( void ) const { return this->_query; }
std::string Request::getPath( void ) const { return this->_path; }
std::string Request::getProtocol(void ) const { return this->_protocol; }
short       Request::getUriExtension( void ) const { return this->_uriExtension; }
std::string Request::getBodyFilename( void ) const { return this->_bodyFilename; }
bool		Request::isComplete( void ) const { return _status; }
std::map<std::string, std::string> const& Request::getHeaders( void ) const { return this->_headers; }
size_t 		Request::getPort( void ) const { return this->_port; }
ServerConfigClass   Request::getConfig( void ) const { return this->_config; }
std::string Request::getFileName( void) const { return this->_fileName; }
short       Request::getPos( void ) const { return this->_pos; }
int         Request::getBodyFd( void) const { return this->_bodyFd; }
int         Request::getTotalBytesRead( void ) const { return this->_totalBytesRead; }
void		Request::setConfig( ServerConfigClass config ){ this->_config = config; }

void    Request::reset( void ) {
    this->_dataGatherer = "";
    this->_method = "";
    this->_uri = "";
    this->_query = "";
    this->_path = "";
    this->_protocol = "";
    this->_uriExtension = 0;
    this->_headers.clear();
    this->_port = 0;
    this->_bodyFilename = "";
    this->_status = false;
    this->_fileName = "";
    close(this->_bodyFd);
    this->_bodyFd = -1;
}

// bool _TEST_FOR_METHODS_ = false;

void	Request::_extractData( void ) {
	std::string	tmp = this->_path;
	if (std::count(tmp.begin(), tmp.end(), '/') == 1) {
		for (size_t i = 0; i < _config.getLocationCount(); i++) {
			if ("/" == _config.getLocationClass()[i].getPath()) {
				this->_fileName = tmp;
				this->_pos = i;
                // _TEST_FOR_METHODS_ = true;
				return ;
			}
		}
	}
	while (420) {
		for (size_t i = 0; i < _config.getLocationCount(); i++) {
			if ((tmp == _config.getLocationClass()[i].getPath() || (tmp + "/") == _config.getLocationClass()[i].getPath()) &&
				this->_method == _config.getLocationClass()[i].getMethod()) {
				try {
					this->_fileName = _fileName.substr(_fileName.find_first_of("/"), _fileName.length());
					this->_pos = i;
                    // _TEST_FOR_METHODS_ = true;
					return ;
				} catch (...) {
					return ;
				}
			}
		}
		try {
			this->_fileName = tmp.substr(tmp.find_last_of("/"), tmp.length()) + _fileName;	
			tmp = tmp.substr(0, tmp.find_last_of("/"));
		} catch ( std::exception &e ) {
			return ;
		}
	}
}

/* -- PUBLIC METHODS */
void    Request::append( const char * recvBuffer, int size ) {
    _dataGatherer = "";
    for (int i = 0; i < size; i++) {
        _dataGatherer += recvBuffer[i];
    }
    return ;
}

void	Request::parse( void ) {
    
	if (_headersComplete() == true || _headersPassed == true) {
        std::stringstream	iss(_dataGatherer);
		if (_headers.empty() == true) {
            
            try {
                _extractRequestLine(iss);
                _extractHeaders(iss);
                _headersPassed = true;
                _extractData();
            }
            catch(const std::exception& e) {
                throw parseErr(e.what());
            }
        }
        if (this->_method == "POST") {
            if (this->_uriExtension == 0) {
                if (this->_config.getLocationClass()[this->_pos].getUpload().empty()) {
                    throw parseErr("403 Forbidden");
                }
            }
            std::map<std::string, std::string>::iterator transferEncoding = _headers.find("Transfer-Encoding");
            if (transferEncoding != _headers.end() && transferEncoding->second == "chunked") {
                if (_bodyComplete() == true) {
                    _handleChunkedRequest(iss);
                    _status = true;
                    return ;
                }
            }
            try {
                _handleBasicRequest(iss);
            }
            catch ( const std::exception &e ) {
                throw (e);
            }
            std::cout << _totalBytesRead << " " << std::stoi(_headers.find("Content-Length")->second) << std::endl;
            if (_totalBytesRead == std::stoi(_headers.find("Content-Length")->second)) {
                _status = true;
            }
            return ;
        }
        else {
            _status = true;
            return ;
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
	if (myvec[0] == "GET" or myvec[0] == "POST" or myvec[0] == "DELETE")
		this->_method = myvec[0];
	else
		throw parseErr("405 Method Not Allowed");
    this->_uri = myvec[1];
    this->_path = this->_uri;
    size_t pos = this->_uri.find("?");
    if (pos != std::string::npos) {
        this->_query = this->_uri.substr(pos + 1, this->_uri.length());
        this->_path.erase(pos, this->_path.length());
    }
    myvec[2] == "HTTP/1.1" ? this->_protocol = myvec[2] : throw parseErr("505 HTTP Version Not Supported");
    if (_hasEnding(this->_uri, ".py")) { this->_uriExtension = PY; }
    else if (_hasEnding(this->_uri, ".php")) { this->_uriExtension = PHP; }
}

void    Request::_extractHeaders( std::stringstream & iss ) {
    std::string line;
    std::vector<std::string> myvec(0);
    while (std::getline(iss, line)) {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        if (line.size() == 0)
            break ;
        myvec.push_back(line.substr(0, line.find(':')));
        myvec.push_back(line.substr(myvec[0].length() + 2, line.length()));
        if ((myvec.at(0).empty() || myvec.at(1).empty()) ||
            _checkHeadersKeySyntax(myvec[0]) == false)
            throw parseErr("400 Bad Request");
        if (this->_headers.find(myvec[0]) == this->_headers.end()) {
            this->_headers[myvec[0]] = myvec[1];
    		if (myvec[0] == "Host" && (myvec[1].find(':') != std::string::npos)) {
                std::string s = myvec[1].substr(myvec[1].find(':') + 1, myvec[1].length());
                (!s.empty() && s.find_first_not_of("0123456789") == std::string::npos) ? this->_port = std::stoi(s) : throw parseErr("400 Bad Request");
            }
        }
        else throw parseErr("400 Bad Request");
        myvec.clear();
    }
    /* ------
        TO COMPLY WITH HTTP/1.1, CLIENTS MUST INCLUDE THE "Host: header" WITH EACH REQUEST
    ------ */
    if (this->_headers.find("host") != this->_headers.end()) { throw parseErr("400 Bad Request"); } // ==?
}

template <class T>
inline std::string Request::_toString( const T& t ) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

void Request::_handleChunkedRequest( std::stringstream & iss ) {
    /* ------
        "Messages MUST NOT include both a Content-Length header field and a non-identity transfer-coding.
        If the message does include a non-identity transfer-coding, the Content-Length MUST be ignored."
        (RFC 2616, Section 4.4)
    ------ */
    std::string line;
    uint16_t n = 0;
    this->_bodyFilename = _config.getLocationClass()[_pos].getRoot() + _config.getLocationClass()[_pos].getUpload() + _fileName;
    // FILE * fptr = fopen(this->_bodyFilename.c_str(), "w");
    if (_bodyFd == -1)
        _bodyFd = open(this->_bodyFilename.c_str(), O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR |  S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    struct pollfd fds = {};
    fds.fd = _bodyFd;
    // this->_bodyFd = fds.fd;
    fds.events = POLLOUT;
    int rc = poll(&fds, 1, 0);
    if (rc < 1)
        ;
    else if (rc == 1 && fds.events & POLLOUT) {
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
                    
                    write(fds.fd, line.c_str(), line.length());
                }
                else
                    write(fds.fd, line.c_str(), line.length());
            }  
        }
    }
    // close(fd);
}

void    Request::_handleBasicRequest( std::stringstream & iss ) {
    /* ------
        IN THIS CASE WE SHOULD HAVE A VALID "Content-Length: header"
        ACCORDING TO RFC 2616 SEC4.4
    ------ */
    if (_checkContentLength() == _CONTENT_LENGTH_NOT_FOUND_ ||
            _checkContentLength() == _CONTENT_LENGTH_NEGATIVE_)
        throw parseErr("400 Bad Request");
    (_uriExtension == 0) ? this->_bodyFilename = _config.getLocationClass()[_pos].getRoot() + _config.getLocationClass()[_pos].getUpload() + _fileName : // NOT_CGI
    this->_bodyFilename = _config.getLocationClass()[_pos].getRoot() + _fileName + _toString(clock()); // CGI
    if (_bodyFd == -1)
        _bodyFd = open(this->_bodyFilename.c_str(), O_CREAT | O_TRUNC | O_RDWR, S_IRUSR |  S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    struct pollfd fds = {};
    fds.fd = _bodyFd;
    fds.events = POLLOUT;
    int rc = poll(&fds, 1, 0);
    if (rc < 1)
        ;
    else if (rc == 1 && fds.events & POLLOUT) {
        std::string str = _toString(iss.rdbuf());
        this->_totalBytesRead += write(fds.fd, str.c_str(), str.length());
    }
    // close(_bodyFd);
}

bool	Request::_headersComplete( void ) {
	return _dataGatherer.find("\r\n\r\n") != std::string::npos;
}

bool    Request::_bodyComplete( void ) {
    return _dataGatherer.find("0\r\n\r\n") != std::string::npos;
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

bool    Request::_checkHeadersKeySyntax( std::string key ) {
    for (size_t i = 0; i < key.length(); i++) {
        if (!((key[i] >= 'a' && key[i] <= 'z') || (key[i] >= 'A' && key[i] <= 'Z') || key[i] == '-'))
            return false;
    }
    return true;
}

bool    Request::_hasEnding( std::string const &fullString, std::string const &ending ) {
    if (fullString.length() >= ending.length()) { return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending)); }
    return false;
}

int     Request::_findFileSize( int fd ) {
    off_t fsize;
    fsize = lseek(fd, 0, SEEK_END);
    return fsize;
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

short   Request::_compareContentLengthWithBody( int fd ) {
    if (std::stoi(this->_headers.find("Content-Length")->second) == _findFileSize(fd)) { return _BODY_COMPLETE_; }
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
	// std::ifstream	body(req.getBodyFilename());
	// std::string line;
	// while (getline(body, line))
	// 	std::cout << line << std::endl;
	return o;
}
