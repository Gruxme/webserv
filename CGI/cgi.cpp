/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-fadi <sel-fadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 12:17:14 by sel-fadi          #+#    #+#             */
/*   Updated: 2022/03/11 05:28:33 by sel-fadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

extern char **environ;

cgi::cgi() : _status("200 OK"), _location(""), _contentLength(0), _output(""), _tmp("") {
	this->scriptType = "/Users/sel-fadi/.brew/bin/php-cgi";
}

cgi::~cgi() {
}

cgi::cgi( cgi const &obj ) { *this = obj; }

cgi& cgi::operator=( cgi const &rhs ) {
    if (this != &rhs)
	{
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

void cgi::setRequest(Request request)
{
	this->_request = request;
	if (_request.getMethod() == "POST")
		this->arg = _request.getConfig().getLocationClass()[_request.getPos()].getRoot() + _request.getFileName();
	else
		this->arg = _request.getConfig().getLocationClass()[_request.getPos()].getRoot() + _request.getFileName();
	std::cout << "--------------- [ " << arg << " ] ------------------\n";
}

std::string cgi::getOsName()
{
	#ifdef __APPLE__
		return "(Mac OSX)";
	#elif __MACH__
		return "(Mac OSX)";
	#elif __linux__
		return "(Linux)";
	#elif __FreeBSD__
		return "(FreeBSD)";
	#elif __unix || __unix__
		return "(Unix)";
	#else
		return "Other";
	#endif
}

std::string cgi::getDate()
{
	char buf[100];
	time_t now = std::time(0);
	struct tm *tm = std::gmtime(&now);
	std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", tm);
	return std::string(buf);
}

void cgi::setEnv()
{
	std::cout << _request.getMethod().c_str() << std::endl;
	if (_request.getMethod() == "POST")
	{
		std::cout << "--------------- ||| --- POST --- ||| ---------------\n";
		setenv("CONTENT_LENGTH", _request.getHeaders().find("Content-Length")->second.c_str(), 1);
		setenv("SERVER_PROTOCOL", _request.getProtocol().c_str(), 1);
		// if (!_request.getQuery().empty()) {
		// 	setenv("QUERY_STRING", _request.getQuery().c_str(), 1);
		// }
		// else {
		// 	setenv("QUERY_STRING", _tmp.c_str(), 1);
		// }
		setenv("REQUEST_METHOD",_request.getMethod().c_str(), 1);
		setenv("REDIRECT_STATUS","200", 1);
		setenv("CONTENT_TYPE", _request.getHeaders().find("Content-Type")->second.c_str(), 1);
		setenv("SCRIPT_FILENAME", arg.c_str(), 1);

		std::cout  <<  _request.getHeaders().find("Content-Length")->second.c_str() << std::endl;
		std::cout << _request.getMethod().c_str() << std::endl;
		std::cout << _request.getHeaders().find("Content-Type")->second.c_str() << std::endl;
		std::cout << arg.c_str() << std::endl;

		
	}
	else if (_request.getMethod() == "GET")
	{
		std::cout << "--------------- ||| ----- GET ----- ||| ---------------\n";
		setenv("SERVER_PROTOCOL", _request.getProtocol().c_str(), 1);
		setenv("QUERY_STRING", _request.getQuery().c_str(), 1);
		setenv("PATH_INFO", _request.getPath().c_str(), 1);		
		setenv("REQUEST_METHOD", _request.getMethod().c_str(), 1);
		setenv("REDIRECT_STATUS","200",1);
		setenv("SCRIPT_FILENAME", arg.c_str(), 1);
	}
}


void cgi::exec_script( std::string filename )
{
    int ret;
    char *tmp[3];
	int fd1;

	int fd = open(filename.c_str() , O_RDWR);
	tmp[0] = (char *)scriptType.c_str();
	tmp[1] = (char *)arg.c_str();
	tmp[2] = NULL;
	setEnv();	
	std::cout << "[ " << _request.getBodyFd() << " ]\n";
	fd1 = open(_request.getBodyFilename().c_str() , O_RDWR , 0777);
	dup2(fd1, 0);
	dup2(fd, 1);
	
    ret = execve(tmp[0], tmp, environ);
	
    if (ret == -1) {
		throw "500 Internal Server Error";
	}
}

void cgi::exec_scriptGET(int fd)
{
	int ret;
    char *tmp[3];
	int fd1;

	tmp[0] = (char*)scriptType.c_str();
	tmp[1] = (char*)arg.c_str();
	tmp[2] = NULL;

	setEnv();
	std::cout << "[ " << _request.getBodyFd() << " ]\n";
	fd1 = open("response.txt" , O_RDWR| O_CREAT | O_TRUNC, 0777);
	dup2(fd1, 0);
	dup2(fd, 1);
    ret = execve(tmp[0], tmp, environ);
    if (ret == -1) {
		throw "500 Internal Server Error";
	}
}

void cgi::processing_cgi(Request request)
{
	if (request.getMethod() == "POST") {	
		int fd;
		pid_t pid;
		std::string filename = "response.txt";	
		fd = open(filename.c_str() , O_RDWR | O_CREAT | O_TRUNC, 0777);
		close(fd);
		setRequest(request);
		int fd5 = open(_request.getBodyFilename().c_str(), O_RDONLY);
		_tmp = _generateTmp(fd5);
		close(fd5);
		
		std::cout << "[ -------- POST ] : ======" <<_request.getBodyFilename().c_str() << std::endl;
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		else if (pid == 0)
			exec_script(filename);
		int	status = 0;
		int ret = 0;
		while (waitpid(-1, &status, 0) > 0)
			if (WIFEXITED(status))
				ret = WEXITSTATUS(status);
		int fd2 = open(filename.c_str(), O_RDONLY);
		parseOutput(fd2);
		close(fd2);
	}
	
	else {
		int fd;
		pid_t pid;
		std::string filename = "response.txt";

		fd = open(filename.c_str() , O_RDWR | O_CREAT | O_TRUNC, 0777);
		setRequest(request);
		std::cout << "[ --------  GET] : " << _request.getQuery().c_str() << std::endl;
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		else if (pid == 0)
			exec_scriptGET(fd);
		wait(NULL);
		close(fd);
		int fd2 = open(filename.c_str(), O_RDONLY);
		parseOutput(fd2);
		close(fd2);
	}
}




/* ABOUNAK SHIT */

std::string	cgi::_generateTmp( int fd ) {
	std::string tmp = "";
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
				tmp += buffer[i];
				if (tmp.find("\r\n\r\n") != std::string::npos) {
					_contentLength++;
				}	
			}
		}
	}
	return tmp;
}


void	cgi::parseOutputGET( int fd ) {
	std::string tmp = this->_generateTmp(fd);
	std::cout << std::endl;
	std::cout << "#### DEBUGGGGOU {TMP} ####" << std::endl;
	std::stringstream ss(tmp);
	std::string buffer;
	std::getline(ss, buffer);
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
	std::cout << _contentLength << std::endl;
	_output += "Content-Length: " + std::to_string(_contentLength) + "\r\nConnection: " + _request.getHeaders().find("Connection")->second + tmp;
}



void	cgi::parseOutput( int fd ) {
	std::string tmp = this->_generateTmp(fd);
	std::cout << std::endl;
	std::cout << "#### DEBUGGGGOU {TMP} ####" << std::endl;
	std::stringstream ss(tmp);
	std::string buffer;
	std::getline(ss, buffer);
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
	std::cout << _contentLength << std::endl;
	_output += "Content-Length: " + std::to_string(_contentLength) + "\r\nConnection: " + _request.getHeaders().find("Connection")->second + "\r\n";
	_output += tmp;
}

std::string	cgi::getContent( void ) const {
	return this->_output;
}