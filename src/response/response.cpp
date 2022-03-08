/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-fadi <sel-fadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/03/08 14:04:22 by sel-fadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

response::response() :
	_headers(""), _body(""), _bodyFd(-1),
	_bodySize(0), _totalSent(0), _headersSent(false),
	_error(false), _autoIndex(false), _config(), _req(), _fileName(""),
	_pos(-1)
	{}
response::~response() {
	//check if sigpipe would need close of fd here
}

response::response( const response &x)
{
	this->operator=(x);
}

response	&response::operator=(const response &x){
	_headers = x._headers;
	_body = x._body;
	_indexList = x._indexList;
	_bodyFd = x._bodyFd;
	_bodySize = x._bodySize;
	_totalSent = x._totalSent;
	_headersSent = x._headersSent;
	_error = x._error;
	_autoIndex = x._autoIndex;
	_config = x._config;
	_req = x._req;
	_fileName = x._fileName;
	_pos = x._pos;
	return *this;
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

bool	response::_autoindexModule(std::string path){
	DIR 			*dir;
	struct dirent	*ent;
	struct stat		status;
	char			*date = new char[20]();
	std::ostringstream dirListHtml;
	dirListHtml << "<html>\n<head><title>Index of " << path << "</title></head>\n<body>\n<h1>Index of "\
				<< path << "</h1>\n<hr><pre><a href=\"../\">../</a>\n";
	if ((dir = opendir(path.c_str())) != NULL){
		while ((ent = readdir(dir)) != NULL){
			std::string	fileName(ent->d_name);
			if(fileName == "." || fileName == "..")
				continue;
			stat((path + fileName).c_str(), &status);
			if(S_ISDIR(status.st_mode))
				fileName += "/";
			dirListHtml << "<a href=\"" << fileName << "\">" << fileName << "</a>";
			strftime(date, 17, "%d-%b-%y %H:%M", gmtime(&(status.st_mtimespec.tv_sec)));
			dirListHtml << std::setw(62 - fileName.length()) << date;
			if(S_ISDIR(status.st_mode))
				dirListHtml << std::setw(21) << std::right << "-\n";
			else{
				dirListHtml << std::setw(20) << status.st_size << std::endl;
			}
		}
		delete[] date;
		dirListHtml << "</pre><hr></body>\n</html>";
		_indexList = dirListHtml.str();
		closedir(dir);
		return true;
	}
		return false;
}

bool	response::isAutoIndex( void ) const{
	return _autoIndex;
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
		if(_pos != -1 && (allowedMethods = _config.getLocationClass()[_pos].getMethod()).empty() == false )
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

std::string		response::indexListContent( void ) const{
	return _indexList;
}

void response::_getResrc( std::string absPath ) {
	if (_req.getUriExtension() == PHP){
		_cgi.processing_cgi(_req);
	}
	else if(_req.getUriExtension() == PY){
		_cgi.processing_cgi(_req);
	}
	else{
		int	fd = -1;
		if((fd = open(absPath.c_str(), O_RDONLY)) < 0){
			if(errno == ENOENT)
				errorMsg("404 Not Found");
			else if(errno == EACCES)
				errorMsg("403 Forbidden");
			else
				errorMsg("500 Internal Server Error");
		}
		else{
			std::ostringstream	res;
			struct stat			status;
			const char *mimeType;

			time_t now = time(0);
			char *date = new char[30]();
			strftime(date, 29, "%a, %d %b %Y %T %Z", gmtime(&now));
			res << "HTTP/1.1 200 OK\r\nDate: " << date << "\r\n" << "Server: Webserv/4.2.0 \r\n";
			free(date);
			stat(absPath.c_str(), &status);
			if(S_ISDIR(status.st_mode)){
				/* if(!_config.getAutoIndex()){
					errorMsg("403 Forbidden");
					return ;
				} */
				_autoIndex = true;
				if(!_autoindexModule(absPath)){
					errorMsg("500 Internal Server Error");
					return;
				}
				res << "Content-Type: text/html\r\n";
				res << "Content-Length: " << (_bodySize = _indexList.length()) << "\r\n";
				_body = "";
			}
			else
			{
				mimeType = MimeTypes::getType(absPath.c_str());
				if (mimeType == NULL)
					res << "Content-Type: text/plain\r\n";
				else
					res << "Content-Type: " << mimeType << "\r\n";
				res << "Content-Length: " << (_bodySize = status.st_size) << "\r\n";
				_body = absPath;
			}
			res << "Connection: " << _req.getHeaders().find("Connection")->second << "\r\n\r\n";
			_headers = res.str();
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
	std::cout << _body.c_str() << " -> sent  " << _totalSent << " bytes of " <<_bodySize << std::endl;
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
