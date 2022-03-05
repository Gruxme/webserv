/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/03/05 15:46:51 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

response::response() :
	_headers(""), _body(""), _bodyFd(-1),
	_bodySize(0), _totalSent(0), _headersSent(false),
	_error(false), _config(), _req(), _fileName(""),
	_pos(-1)
	{}
response::~response() {
	//check if sigpipe would need close of fd here
}

void	response::headersSent(){
	_headersSent = true;
}

bool	response::getHeaderStatus() const{
	return _headersSent;
}

void	response::offsetCursor(off_t offset){
	lseek(_bodyFd, offset, SEEK_CUR);
}

void	response::errorMsg( std::string type){
	std::ostringstream	errRes;
	struct stat			status;
	std::string			errorFile;
	std::string			statusCode = type.substr(0, 3);

	// if(*(_config.getErrorPage().rbegin().base()) == '/')
	// 	errorFile = _config.getErrorPage() + statusCode + ".html"; // Maybe do this in config parsing
	// else
	errorFile = _config.getErrorPage() + statusCode + ".html";
	errRes << "HTTP/1.1 " << type << "\r\nDate: ";
	time_t	now = time(0);
	char	*date = new char[30]();
	strftime(date, 29, "%a, %d %b %Y %T %Z", gmtime(&now));
	errRes << date << "\r\n" << "Server: Webserv/4.2.0 \r\n";
	free(date);
	if(statusCode == "405"){
		std::string allowedMethods;
		//check allowed method in location if any first
		if(_pos == -1 && (allowedMethods = _config.getLocationClass()[_pos].getMethod()).empty() == false )
			errRes << "Allow: " << allowedMethods << "\r\n";
		errRes << "Allow: GET, POST, DELETE\r\n";
	}
	errRes << "Content-Type: text/html\r\n";
	std::cout << errorFile << std::endl;
	if(stat(errorFile.c_str(), &status) < 0){
		errorFile = "/Users/abiari/Desktop/webserv/errorPages/" + statusCode + ".html";
		stat(errorFile.c_str(), &status);
	}
	errRes << "Content-Length: " << (_bodySize = status.st_size) << "\r\n";
	errRes << "Connection: closed\r\n\r\n";
	_headers = errRes.str();
	_body = errorFile;
	_error = true;
}

void response::_getResrc( std::string absPath ) {
	if (_req.getUriExtension() == PHP){

	}
	else if(_req.getUriExtension() == PY){
		
	}
	else{
		int	fd = -1;
		if((fd = open(absPath.c_str(), O_RDONLY)) < 0){
			if(errno == ENOENT)
				errorMsg("404 Not Found");
			else if(errno == EACCES)
				errorMsg("403 Forbidden");
			// else if(errno == EISDIR){
			// 	if(_config.getAutoIndex()){
			// 		//launch autoindex module
			// 	}
			// 	else
			// 		errorMsg("403 Forbidden");
			// }
			else
				errorMsg("500 Internal Server Error");
		}
		else{
			std::ostringstream	res;
			struct stat			status;
			const char *mimeType = MimeTypes::getType(absPath.c_str());

			time_t now = time(0);
			char *date = new char[30]();
			strftime(date, 29, "%a, %d %b %Y %T %Z", gmtime(&now));
			res << "HTTP/1.1 200 OK\r\nDate: " << date << "\r\n" << "Server: Webserv/4.2.0 \r\n";
			free(date);
			if(mimeType == NULL)
				res << "Content-Type: text/plain\r\n";
			else
				res << "Content-Type: " << mimeType << "\r\n";
			stat(absPath.c_str(), &status);
			res << "Content-Length: " << (_bodySize = status.st_size) << "\r\n";
			res << "Connection: " << _req.getHeaders().find("Connection")->second << "\r\n\r\n";
			_headers = res.str();
			_body = absPath;
		}
	}
    return ;
}


void		response::_postResrc( std::string absPath ){
	(void)absPath;
}
void		response::_deleteResrc( std::string absPath ){
	(void)absPath;
}

void		response::setBytesSent(size_t bytesSent){
	_totalSent += bytesSent;
}

size_t		response::getBodySize( void ) const{
	return _bodySize;
}

bool		response::bodyEof( void ) const{
	std::cout << _body.c_str() << " " << _totalSent << " " <<_bodySize << std::endl;
	if(_totalSent == _bodySize){
		close(_bodyFd);
		return true;
	}
	return false;
}

std::string	response::getBodyContent( void ){
	struct pollfd		fds = {};
	char				buff[2028*100];
	std::string			content("");
	int					rc = 0;
	if(_bodyFd == -1)
	{
		_bodyFd = open(_body.c_str(), O_RDONLY); // make it non block
		fcntl(_bodyFd, F_SETFL, O_NONBLOCK);
	}
	fds.fd = _bodyFd;
	fds.events = POLLIN;
	if ((poll(&fds, 1, -1) > 0) && (fds.revents = POLLIN))
	{
		bzero(&buff, 2028*100);
		if ((rc = read(fds.fd, &buff, 2028*100)) > 0){
			for (int i = 0; i < rc; i++)
				content += buff[i];
		}
	}
	return content;
}

// void	response::_extractData( void ) {
// 	std::string tmpPath = this->_req.getPath();
// 	std::string path = this->_req.getPath();
// 	int ret = 0;
// 	while (path.find("/") != std::string::npos) {
// 		_path = path;
// 		for (size_t i = 0; i < _config.getLocationCount(); i++) {
// 			if ("/" == _config.getLocationClass()[i].getPath() && _config.getLocationClass()[i].getMethod() == _req.getMethod()) {
// 				ret = i;
// 				this->_pos = ret;
// 			}
// 			else if (path == _config.getLocationClass()[i].getPath() && _config.getLocationClass()[i].getMethod() == _req.getMethod() ) {
// 				this->_fileName = tmpPath.substr(this->_path.length(), tmpPath.length());
// 				if (this->_fileName == "/")
// 					this->_fileName = "";
// 				else if (this->_fileName.find("/") != std::string::npos) {
// 					this->_fileName.erase(this->_fileName.find_first_of("/"));
// 				}
// 				ret = i;
// 				this->_pos = ret;
// 				return ;
// 			}
// 		}
// 		if (path.find_first_of("/") == path.find_last_of("/")) {
// 			// throw "9AWD";
// 			this->_pos = ret;
// 			this->_path = _config.getLocationClass()[this->_pos].getRoot();
// 			this->_fileName = tmpPath.substr(tmpPath.find("/") + 1, tmpPath.length());
// 			return ;
// 		}
// 		else
// 			path = path.substr(0, path.find_last_of("/"));
// 	}
// 	return ;
// }

void	response::_extractData( void ) {
	std::string	tmp = this->_req.getPath();
	if (std::count(tmp.begin(), tmp.end(), '/') == 1) {
		for (size_t i = 0; i < _config.getLocationCount(); i++) {
			if ("/" == _config.getLocationClass()[i].getPath()) {
				this->_fileName = tmp;
				this->_pos = i;
				return ;
			}
		}
	}
	while (420) {
		for (size_t i = 0; i < _config.getLocationCount(); i++) {
			if ((tmp == _config.getLocationClass()[i].getPath() || (tmp + "/") == _config.getLocationClass()[i].getPath()) &&
				this->_req.getMethod() == _config.getLocationClass()[i].getMethod()) {
				try {
					this->_fileName = _fileName.substr(_fileName.find_first_of("/"), _fileName.length());
					this->_pos = i;
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

bool	response::isError( void ) const{
	return _error;
}

void response::serveRequest( void ) {
	this->_extractData();
	/* to be changed */
	if (this->_pos == -1)
		exit(0);
	std::string absolutePath = _config.getLocationClass()[this->_pos].getRoot() + _fileName;
	if(_req.getMethod() == "GET")
		_getResrc(absolutePath);
	else if (_req.getMethod() == "POST")
		_postResrc(absolutePath);
	else if(_req.getMethod() == "DELETE")
		_deleteResrc(absolutePath);
	else
		errorMsg("405 Method Not Allowed");
    return ; 
}

void		response::setData( ServerConfigClass config, Request req ){
	_config = config;
	_req = req;
}

std::string	response::getHeaders( void ) const { return this->_headers; }
std::string	response::getBody( void ) const { return this->_body; }
ServerConfigClass	response::getConfig( void ) const { return this->_config; }
Request	response::getRequest( void ) const { return this->_req; }
std::string	response::getFileName( void ) const { return this->_fileName; }
int			response::getPos( void ) const { return this->_pos; }
