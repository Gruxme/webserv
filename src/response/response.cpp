/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/02/27 18:37:50 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"


response::response() {}
response::~response() {}

void	response::_errorMsg( std::string type , std::string statusCode){
	std::ostringstream	errRes;
	struct stat			status;
	std::string			errorFile;

	if(*(_config.getErrorPage().rbegin().base()) == '/')
		errorFile = _config.getErrorPage() + statusCode + ".html"; // Maybe do this in config parsing
	else
		errorFile = _config.getErrorPage() + "/" + statusCode + ".html";
	errRes << "HTTP /1.1 " << type << "\r\nDate: ";
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
	if(stat(errorFile.c_str(), &status) < 0){
		errorFile = "/errorPages/" + statusCode + ".html";
		if(stat(errorFile.c_str(), &status)){
			perror("stat: ");
			exit(EXIT_FAILURE);
		}
	}
	errRes << "Content-Length: " << (_bodySize = status.st_size) << "\r\n";
	errRes << "Connection: closed\r\n\r\n";
	_headers = errRes.str();
	_headersStatus = true;
	_body = errorFile;
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
				_errorMsg("404 Not Found", "404");
			else if(errno == EACCES)
				_errorMsg("403 Forbidden", "403");
			else if(errno == EISDIR){
				if(_config.getAutoIndex()){
					//launch autoindex module
				}
				else
					_errorMsg("403 Forbidden", "403");
			}
			else
				_errorMsg("500 Internal Server Error", "500");
		}
		else{
			std::ostringstream	res("HTTP /1.1 200 OK\r\nDate: ");
			struct stat			status;
			const char *mimeType = MimeTypes::getType(absPath.c_str());

			time_t now = time(0);
			char *date = new char[30]();
			strftime(date, 29, "%a, %d %b %Y %T %Z", gmtime(&now));
			res << date << "\r\n" << "Server: Webserv/4.2.0 \r\n";
			free(date);
			if(mimeType == NULL)
				res << "Content-Type: text/plain\r\n";
			else
				res << "Content-Type: " << mimeType << "\r\n";
			if (stat(absPath.c_str(), &status) < 0)
			{
				perror("stat: "); // define behaviour if error file defined in config do not actually exist
				exit(EXIT_FAILURE);
			}
			res << "Content-Length: " << (_bodySize = status.st_size) << "\r\n";
			res << "Connection: " << (_status = (_req.getHeaders().find("Connection")->second != "close")) << "\r\n\r\n";
			_headersStatus = true;
		}
	}
    return ;
}

void		response::setSendStatus(bool status){
	_sendStatus = status;
}

bool		response::getSendStatus( void ){
	return _status;
}

std::string	response::getBodyContent( void ){
	struct pollfd		fds = {};
	char				buff[4097];
	std::string			content("");
	_bodyFd = open(_body.c_str(), O_RDONLY); //make it non block
	fds.fd = _bodyFd;
	fds.events = POLLIN;
	if ((poll(&fds, 1, -1) > 0) && (fds.revents = POLLIN))
	{
		bzero(&buff, 4097);
		if (read(fds.fd, &buff, 4096) > 0)
			content = buff;
	}
	return content;
	//above block to be set along headers if data is small enough not to be chunked
	// for chunked response open file alongside header and send first chunk then send remaining chunks in here
}

void	response::_extractData( void ) {
	std::string tmpPath = this->_req.getPath();
	std::string path = this->_req.getPath();
	while (path.find("/") != std::string::npos) {
		_path = path;
		for (int i = 0; i < _config.getLocationCount(); i++) {
			if ("/" == _config.getLocationClass()[i].getPath() && _config.getLocationClass()[i].getMethod() == _req.getMethod())
				this->_pos = i;
			else if (path == _config.getLocationClass()[i].getPath() && _config.getLocationClass()[i].getMethod() == _req.getMethod() ) {
				this->_scriptName = tmpPath.substr(this->_path.length(), tmpPath.length());
				if (this->_scriptName == "/")
					this->_scriptName = "";
					
				else if (this->_scriptName.find("/") != std::string::npos) {
					this->_scriptName.erase(this->_scriptName.find_first_of("/"));
				}
				this->_pos = i;
				return ;
			}
		}
		if (path.find_first_of("/") == path.find_last_of("/")) {
			this->_path = "/";
			this->_scriptName = tmpPath.substr(tmpPath.find("/") + 1, tmpPath.length());
			return ;
		}
		else
			path = path.substr(0, path.find_last_of("/"));
	}
	return ;
}

void response::serveRequest( void ) {
	this->_extractData();
	if(_req.getMethod() == "GET")
		_getResrc(_path + _scriptName);
	else if (_req.getMethod() == "POST")
		_postResrc(_path + _scriptName);
	else if(_req.getMethod() == "DELETE")
		_deleteResrc(_path + _scriptName);
	else
		_errorMsg("405 Method Not Allowed", "405");
    return ; 
}

bool		response::connStatus( void ) { return _status; }

void		response::setData( ServerConfigClass config, Request req ){
	_config = config;
	_req = req;
}
