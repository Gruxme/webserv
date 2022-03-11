/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-fadi <sel-fadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/03/11 13:14:20 by sel-fadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

response::response() :
	_headers(""), _body(""), _bodyFd(-1),
	_bodySize(0), _totalSent(0), _headersSent(false),
	_error(false), _autoIndex(false), _req(), _cgi() {}

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
	_req = x._req;
	_cgi = x._cgi;
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

void	response::errorMsg( std::string type ){
	std::ostringstream	errRes;
	struct stat			status;
	std::string			errorFile;
	std::string			statusCode = type.substr(0, 3);

	errorFile = _req.getConfig().getErrorPage() + statusCode + ".html";
	errRes << "HTTP/1.1 " << type << "\r\nDate: ";
	time_t	now = time(0);
	char	*date = new char[30]();
	strftime(date, 29, "%a, %d %b %Y %T %Z", gmtime(&now));
	errRes << date << "\r\n" << "Server: Webserv/4.2.0 \r\n";
	delete[] date;
	if(statusCode == "405"){
		std::string allowedMethods;
		//check allowed method in location if any first
		int pos = _req.getPos();
		if (pos != -1 && (allowedMethods = _req.getConfig().getLocationClass()[pos].getMethod()).empty() == false )
			errRes << "Allow: " << allowedMethods << "\r\n";
		else
			errRes << "Allow: GET, POST, DELETE\r\n";
	}
	errRes << "Content-Type: text/html\r\n";
	std::cout << errorFile << std::endl;
	if(stat(errorFile.c_str(), &status) < 0){
		char *cwd = getcwd(NULL, 0);
		errorFile = cwd;
		errorFile += "/errorPages/" + statusCode + ".html";
		stat(errorFile.c_str(), &status);
		delete(cwd);
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
	if (_req.getUriExtension() == PHP || _req.getUriExtension() == PY){
		_cgi.processing_cgi(_req);
		_headersSent = true;
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
			close(fd);
			std::ostringstream	res;
			struct stat			status;
			const char *mimeType;

			time_t now = time(0);
			char *date = new char[30]();
			strftime(date, 29, "%a, %d %b %Y %T %Z", gmtime(&now));
			res << "HTTP/1.1 200 OK\r\nDate: " << date << "\r\n" << "Server: Webserv/4.2.0 \r\n";
			delete[] date;
			stat(absPath.c_str(), &status);
			if (S_ISDIR(status.st_mode)){
				if (!_req.getConfig().getLocationClass()[_req.getPos()].getAutoIndex()) {
					errorMsg("403 Forbidden");
					return ;
				}
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


void		response::_postResrc() {
	if (_req.getUriExtension() == PHP || _req.getUriExtension() == PY){
		_cgi.processing_cgi(_req);
		_headersSent = true;
	}
	else {
		std::ostringstream	res;

		time_t now = time(0);
		char *date = new char[30]();
		strftime(date, 29, "%a, %d %b %Y %T %Z", gmtime(&now));
		res << "HTTP/1.1 201 CREATED\r\nDate: " << date << "\r\n" << "Server: Webserv/4.2.0 \r\n";
		delete[] date;
		res << "Connection: " << _req.getHeaders().find("Connection")->second << "\r\n\r\n";
		_headers = res.str();
		_bodySize = 0;
	}
}
void		response::_deleteResrc( std::string absPath ){
	if (_req.getUriExtension() == PHP || _req.getUriExtension() == PY){
		_cgi.processing_cgi(_req);
		_headersSent = true;
	}
	else {
		if(remove(absPath.c_str()) < 0){
			if(errno == ENOENT)
				errorMsg("404 Not Found");
			else
				errorMsg("403 Forbidden");
		}
		else{
			std::ostringstream res;

			time_t now = time(0);
			char *date = new char[30]();
			strftime(date, 29, "%a, %d %b %Y %T %Z", gmtime(&now));
			res << "HTTP/1.1 204 No Content\r\nDate: " << date << "\r\n"
				<< "Server: Webserv/4.2.0 \r\n";
			delete[] date;
			res << "Connection: " << _req.getHeaders().find("Connection")->second << "\r\n\r\n";
			_headers = res.str();
			_bodySize = 0;
		}
	}
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

	if(_req.getMethod() != "GET" && !_error)
		return "";
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

bool	response::isError( void ) const{
	return _error;
}

void response::serveRequest( void ) {
	if (_req.getMethod() == _req.getConfig().getLocationClass()[_req.getPos()].getMethod()) {
		if (_req.getMethod() == "GET")
			_getResrc(_req.getConfig().getLocationClass()[_req.getPos()].getRoot() + _req.getFileName());
		else if (_req.getMethod() == "POST") // take upload path and upload filename instead
			_postResrc();
		else if(_req.getMethod() == "DELETE")
			_deleteResrc(_req.getConfig().getLocationClass()[_req.getPos()].getRoot() + _req.getFileName());
	}
	else
		errorMsg("405 Method Not Allowed");
    return ; 
}

void		response::setData( Request req ){
	_req = req;
}

std::string	response::getHeaders( void ) const { return this->_headers; }
std::string	response::getBody( void ) const { return this->_body; }
Request	response::getRequest( void ) const { return this->_req; }
cgi			response::getCgi( void ) const { return _cgi;}
