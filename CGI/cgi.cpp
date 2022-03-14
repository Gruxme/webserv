/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 12:17:14 by sel-fadi          #+#    #+#             */
/*   Updated: 2022/03/14 17:50:21 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

extern char **environ;

cgi::cgi() : _status("200 OK"), _location(""), _contentLength(0), _output(""), _tmp("") {}
cgi::~cgi() {}
cgi::cgi( cgi const &obj ) { *this = obj; }
cgi& cgi::operator=( cgi const &rhs ) {
    if (this != &rhs) {
        this->_request = rhs._request;
        this->arg = rhs.arg;
        this->scriptType = rhs.scriptType;
		this->_status = rhs._status;
		this->_location = rhs._location;
		this->_contentLength = rhs._contentLength;
		this->_output = rhs._output;
		this->_tmp = rhs._tmp;
    }
    return *this;
}

void cgi::_setRequest( Request request ) {
	this->_request = request;
	std::string path = _request.getConfig().getLocationClass()[request.getPos()].getCgi()[1];
	this->arg = _request.getConfig().getLocationClass()[_request.getPos()].getRoot() + _request.getFileName();
	this->scriptType = _request.getConfig().getLocationClass()[_request.getPos()].getCgi()[1];
	_setEnv();
}

std::string cgi::_getDate()
{
	char buf[100];
	time_t now = std::time(0);
	struct tm *tm = std::gmtime(&now);
	std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", tm);
	return std::string(buf);
}

void cgi::_setEnv()
{
	if (_request.getHeaders().find("Cookie") != _request.getHeaders().end())
		setenv("HTTP_COOKIE", _request.getHeaders().find("Cookie")->second.c_str(), 1);
	setenv("CONTENT_LENGTH", _request.getHeaders().find("Content-Length")->second.c_str(), 1);
	setenv("SERVER_PROTOCOL", _request.getProtocol().c_str(), 1);
	setenv("REQUEST_METHOD", _request.getMethod().c_str(), 1);
	setenv("REDIRECT_STATUS","true", 1);
	if (_request.getHeaders().find("Content-Type") != _request.getHeaders().end())
		setenv("CONTENT_TYPE", _request.getHeaders().find("Content-Type")->second.c_str(), 1);
	else
		setenv("CONTENT-TYPE", "text/html; charset=utf-8", 1);
	setenv("SCRIPT_FILENAME", arg.c_str(), 1);
	setenv("SERVER_PROTOCOL", _request.getProtocol().c_str(), 1);
	setenv("QUERY_STRING", _request.getQuery().c_str(), 1);
	setenv("PATH_INFO", _request.getPath().c_str(), 1);		
	setenv("REQUEST_METHOD", _request.getMethod().c_str(), 1);
	setenv("REDIRECT_STATUS","true",1);
	setenv("SCRIPT_FILENAME", arg.c_str(), 1);
}

void cgi::_exec_script( std::string filename )
{
    int ret;
    char *tmp[3];
	int fd1;

	int fd = open(filename.c_str() , O_RDWR);
	tmp[0] = (char *)scriptType.c_str();
	tmp[1] = (char *)arg.c_str();
	tmp[2] = NULL;
	fd1 = open(_request.getBodyFilename().c_str() , O_RDWR , 0777);
	dup2(fd1, 0);
	dup2(fd, 1);
    ret = execve(tmp[0], tmp, environ);
	remove(_request.getBodyFilename().c_str());
	std::cerr << "execve failed with ret: " << ret << " and error of " << strerror(errno) << std::endl;
	exit(errno);
}

void cgi::_exec_scriptGET(int fd)
{
	int ret;
    char *tmp[3];
	int fd1;

	tmp[0] = (char*)scriptType.c_str();
	tmp[1] = (char*)arg.c_str();
	tmp[2] = NULL;

	fd1 = open(_tmpOutputFileName.c_str() , O_RDWR | O_CREAT | O_TRUNC, 0777);
	dup2(fd1, 0);
	dup2(fd, 1);
    ret = execve(tmp[0], tmp, environ);
	remove(_tmpOutputFileName.c_str());
	std::cerr << "execve failed with ret: " << ret << " and error of " << strerror(errno) << std::endl;
	exit(errno);
}

int	_parent( pid_t pid ) {
	int status;
	int ret = 0;
	while (waitpid(pid, &status, 0) > 0)
		if (WIFEXITED(status))
			ret = WEXITSTATUS(status);
	return ret;
}

void cgi::processing_cgi( Request request )
{
	_setRequest(request);
	_tmpOutputFileName = "response" + std::to_string(clock());
	int fd;
	pid_t pid;
	fd = open(_tmpOutputFileName.c_str() , O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (request.getMethod() == "POST") {
		close(fd);
		int fd5 = open(_request.getBodyFilename().c_str(), O_RDONLY);
		_tmp = _generateTmp(fd5);
		close(fd5);
		pid = fork();
		if (pid == -1)
			throw "500 Internal Server Error";
		else if (pid == 0) {
			_exec_script(_tmpOutputFileName);			
		}
	}
	else {
		pid = fork();
		if (pid == -1)
			throw "500 Internal Server Error";
		else if (pid == 0) {
			_exec_scriptGET(fd);			
		}
		close(fd);
	}
	if (_parent(pid))
		throw "500 Internal Server Error"; // waitpid doesn't catch the return of the process
	int fd2 = open(_tmpOutputFileName.c_str(), O_RDONLY);
	_parseOutput(fd2);
	remove(_request.getBodyFilename().c_str());
	close(fd2);
}

std::string	cgi::_generateTmp( int fd ) {
	std::string tmp = "";
	bool wasHere = false;
	struct pollfd  fds = {};
	char buffer[4096];
	bzero(&buffer, 4096);
	int count = 0;
	fds.fd = fd;
	fds.events = POLLIN;
	int rc = poll(&fds, 1, 0);
	if (rc == 1 && fds.events & POLLIN ) {
		while ((count = read(fds.fd, buffer, 4096)) > 0) {
			for (int i = 0; i < count; i++) {
				wasHere = true;
				tmp += buffer[i];	
			}
		}
	}
	if (wasHere == true) _contentLength =  tmp.substr(tmp.find("\r\n\r\n") + 4, tmp.length()).length();
	return tmp;
}

void	cgi::_parseOutput( int fd ) {
	std::string tmp = this->_generateTmp(fd);
	std::stringstream ss(tmp);
	std::string buffer;
	std::getline(ss, buffer);;

	if (std::strncmp("Status: ", buffer.c_str(), 8) == 0) {
		this->_status = buffer.substr(buffer.find("Status: ") + strlen("Status: "));
		tmp.erase(tmp.begin(), tmp.begin() + buffer.length());
	}
	_output += "HTTP/1.1 " + _status + "\r\nDate: ";
	time_t now = time(0);
	char *date = new char[30]();
	strftime(date, 29, "%a, %d %b %Y %T %Z", gmtime(&now));
	_output += date;
	_output += "\r\nServer: Webserv/4.2.0 \r\n";
	delete[] date;
	_output += "Content-Length: " + std::to_string(_contentLength) + "\r\nConnection: " + _request.getHeaders().find("Connection")->second + "\r\n";
	if(tmp.find("\r\n\r\n") == std::string::npos)
		_output += "\r\n";
	_output += tmp;
	remove(_tmpOutputFileName.c_str());
}

std::string	cgi::getContent( void ) const {
	return this->_output;
}
