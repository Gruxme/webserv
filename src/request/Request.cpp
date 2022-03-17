/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:45:08 by aabounak          #+#    #+#             */
/*   Updated: 2022/03/17 18:29:34 by abiari           ###   ########.fr       */
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
    _uriExtension(""),
    _bodyFilename(""),
	_status(false),
    _config(),
	_tmpConfigs(),
    _fileName(""),
    _pos(0),
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
        this->_bodyFilename = rhs._bodyFilename;
        this->_status = rhs._status;
        this->_config = rhs._config;
		this->_tmpConfigs = rhs._tmpConfigs;
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
std::string	Request::getUriExtension( void ) const { return this->_uriExtension; }
std::string Request::getBodyFilename( void ) const { return this->_bodyFilename; }
bool		Request::isComplete( void ) const { return _status; }
std::map<std::string, std::string> const& Request::getHeaders( void ) const { return this->_headers; }
ServerConfigClass   Request::getConfig( void ) const { return this->_config; }
std::string Request::getFileName( void) const { return this->_fileName; }
short       Request::getPos( void ) const { return this->_pos; }
int         Request::getBodyFd( void) const { return this->_bodyFd; }
int         Request::getTotalBytesRead( void ) const { return this->_totalBytesRead; }
void		Request::setConfigs( std::vector<ServerConfigClass> configs ) { this->_tmpConfigs = configs; }

void    Request::reset( void ) {
    this->_dataGatherer = "";
    this->_method = "";
    this->_uri = "";
    this->_query = "";
    this->_path = "";
    this->_protocol = "";
    this->_uriExtension = "";
    this->_headers.clear();
    this->_bodyFilename = "";
    this->_status = false;
    this->_fileName = "";
    this->_totalBytesRead = 0;
    close(this->_bodyFd);
    this->_bodyFd = -1;
}

// bool _TEST_FOR_METHODS_ = false;

void	Request::_extractData( void ) {
	std::string	tmp = this->_path;
	struct stat	status;
	if (std::count(tmp.begin(), tmp.end(), '/') == 1) {
		for (size_t i = 0; i < _config.getLocationCount(); i++) {
			if ("/" == _config.getLocationClass()[i].getPath()) {
				std::vector<std::string> v = _config.getLocationClass()[i].getMethods();
				this->_pos = i;
				if (std::find(v.begin(), v.end(), _method) == v.end())
					throw parseErr("405 Method Not Allowed");
				if(!_config.getLocationClass()[i].getRedirect().empty())
					return ;
				if(_method != "POST" && stat((_config.getLocationClass()[_pos].getRoot() + _fileName).c_str(), &status) < 0)
					throw parseErr("404 Not Found");
				this->_fileName = tmp;
				return ;
			}
		}
	}
	while (420) {
		for (size_t i = 0; i < _config.getLocationCount(); i++) {
			if ((tmp == _config.getLocationClass()[i].getPath() || (tmp + "/") == _config.getLocationClass()[i].getPath())) {
				std::vector<std::string> v = _config.getLocationClass()[i].getMethods();
				this->_pos = i;
				if (std::find(v.begin(), v.end(), _method) == v.end())
					throw parseErr("405 Method Not Allowed");
				if(!_config.getLocationClass()[i].getRedirect().empty())
					return ;
				if(_method != "POST" && stat((_config.getLocationClass()[_pos].getRoot() + _fileName).c_str(), &status) < 0)
					throw parseErr("404 Not Found");
				try {
					if(!this->_fileName.empty())
					{
						this->_fileName = _fileName.substr(_fileName.find_first_of("/"), _fileName.length());
						return;
					}
				} catch (std::exception &e) {
					throw parseErr("500 Internal Server Error") ;
				}
			}
		}
		try {
			this->_fileName = tmp.substr(tmp.find_last_of("/"), tmp.length()) + _fileName;	
			tmp = tmp.substr(0, tmp.find_last_of("/"));
		} catch ( std::exception &e ) {
			throw parseErr("500 Internal Server Error");
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

void	Request::_setConfig( void ) {
	std::string srvName = this->_headers.find("Host")->second;
	// this->_config.~ServerConfigClass();
	for (size_t i = 0; i < _tmpConfigs.size(); i++) {
		if (srvName == _tmpConfigs[i].getServerName()) {
			this->_config = _tmpConfigs[i];
			return ;
		}
	}
	this->_config = _tmpConfigs[0];
}

void	Request::parse( void ) {
    
	if (_headersComplete() == true || _headersPassed == true) {
        std::stringstream	iss(_dataGatherer);
		if (_headers.empty() == true) {
            
            try {
                _extractRequestLine(iss);
                _extractHeaders(iss);
                _headersPassed = true;
				_setConfig();
                _extractData();
            }
            catch(const std::exception& e) {
                throw parseErr(e.what());
            }
        }
        if (this->_method == "POST") {
            if (this->_uriExtension.empty()) {
                if (this->_config.getLocationClass()[this->_pos].getUpload().empty()) {
                    throw parseErr("403 Forbidden");
                }
            }
            std::map<std::string, std::string>::iterator transferEncoding = _headers.find("Transfer-Encoding");
            if (transferEncoding != _headers.end() && transferEncoding->second == "chunked") {
                if (_bodyComplete() == true) {
                    try {
                        _handleChunkedRequest(iss);
                        _status = true;
                        return ;
                    }
                    catch( const std::exception& e ) {
                        throw parseErr(e.what());
                    }
                }
            }
			if(std::stoul(_headers.find("Content-Length")->second) > _config.getBodySizeLimit())
				throw parseErr("413 Payload Too Large");
            try {
                _handleBasicRequest(iss);
            }
            catch ( const std::exception &e ) {
                throw parseErr(e.what());
            }
            std::cout << _totalBytesRead << " " << std::stoul(_headers.find("Content-Length")->second) << std::endl;
            if (_totalBytesRead == std::stol(_headers.find("Content-Length")->second)) {
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
    if (myvec.size() != 3)
        throw parseErr("400 Bad Request");
	if (myvec[0] == "GET" or myvec[0] == "POST" or myvec[0] == "DELETE")
		this->_method = myvec[0];
    this->_uri = myvec[1];
    this->_path = this->_uri;
    size_t pos = this->_uri.find("?");
    if (pos != std::string::npos) {
        this->_query = this->_uri.substr(pos + 1, this->_uri.length());
        this->_path.erase(pos, this->_path.length());
    }
    myvec[2] == "HTTP/1.1" ? this->_protocol = myvec[2] : throw parseErr("505 HTTP Version Not Supported");
    if (_hasEnding(this->_uri, ".py")) { this->_uriExtension = ".py"; }
    else if (_hasEnding(this->_uri, ".php")) { this->_uriExtension = ".php"; }
}

void    Request::_extractHeaders( std::stringstream & iss ) {
    std::string line;
    std::vector<std::string> myvec(0);
    while (std::getline(iss, line)) {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        if (line.size() == 0)
            break ;
        myvec.push_back(line.substr(0, line.find(':')));
        myvec.push_back(line.substr(myvec[0].length() + 1, line.length()));
        if ((myvec.at(0).empty() || myvec.at(1).empty()) ||
            _checkHeadersKeySyntax(myvec[0]) == false || (myvec[1][0] != ' ' || myvec[1][1] == ' ')) // Server does not implement HT check
            throw parseErr("400 Bad Request");
        if (this->_headers.find(myvec[0]) != this->_headers.end())
            throw parseErr("400 Bad Request");
        if (this->_headers.find(myvec[0]) == this->_headers.end()) {
    		if (myvec[0] == "Host" && (myvec[1].find(':') != std::string::npos)) {
				myvec[1] = myvec[1].substr(0, myvec[1].find(':'));
            }
			this->_headers[myvec[0]] = _trim(myvec[1], " ");
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
    (_uriExtension.empty()) ? _config.getLocationClass()[_pos].getUpload() + _fileName : // NOT_CGI
    this->_bodyFilename = _config.getLocationClass()[_pos].getRoot() + _fileName + _toString(clock()); // CGI
    if (_bodyFd == -1)
        _bodyFd = open(this->_bodyFilename.c_str(), O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR |  S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (_bodyFd < 0) {
        if (errno == EISDIR)
            throw parseErr("403 Forbidden");
		else
			throw parseErr("505 Internal Server Error");
    }
    struct pollfd fds = {};
    fds.fd = _bodyFd;
    fds.events = POLLOUT;
    int rc = poll(&fds, 1, 0);
    if (rc < 1) {
        ;
    }
    else if (fds.events & POLLOUT) {
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
                else {
                    write(fds.fd, line.c_str(), line.length());
                }
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
    (_uriExtension.empty()) ? this->_bodyFilename = _config.getLocationClass()[_pos].getRoot() + _config.getLocationClass()[_pos].getUpload() + _fileName : // NOT_CGI
    this->_bodyFilename = _config.getLocationClass()[_pos].getRoot() + _fileName + _toString(clock()); // CGI
    if (_bodyFd == -1)
        _bodyFd = open(this->_bodyFilename.c_str(), O_CREAT | O_TRUNC | O_RDWR, S_IRUSR |  S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if(_bodyFd < 0){
        if(errno == EISDIR)
            throw parseErr("403 Forbidden");
		else
			throw parseErr("500 Internal Server Error");
	}
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

bool    Request::_checkContentLength( void ) {
    if (this->_headers.find("Content-Length") != this->_headers.end()) {
        size_t contentLength = 0;
        try {
            contentLength = std::stoul(this->_headers.find("Content-Length")->second);
            return (contentLength >= 0 ? _CONTENT_LENGTH_FOUND_ : _CONTENT_LENGTH_NEGATIVE_);
        } catch ( std::exception const &e ) {
			throw parseErr("500 Internal Server Error");
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
