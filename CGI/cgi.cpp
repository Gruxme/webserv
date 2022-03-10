/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 12:17:14 by sel-fadi          #+#    #+#             */
/*   Updated: 2022/03/10 20:59:29 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

extern char **environ;

cgi::cgi() {
	std::string	s = _request.getConfig().getLocationClass()[_request.getPos()].getCgiExt();
	std::vector<std::string> myvec = _request.getConfig().getLocationClass()[_request.getPos()].split(s, ' ');
	this->scriptType = myvec[1];
}

cgi::~cgi() {
}

cgi::cgi( cgi const &obj ) { *this = obj; }

cgi& cgi::operator=( cgi const &rhs ) {
    if (this != &rhs)
	{
        this->_request = rhs._request;
        this->getOrPost = rhs.getOrPost;
        this->documentOrRedirection = rhs.documentOrRedirection;
        this->queryString = rhs.queryString;
        this->arg = rhs.arg;
        this->scriptType = rhs.scriptType;
        this->_buffer = rhs._buffer;
    }
    return *this;
}

void cgi::setRequest(Request request, std::string absPath)
{
	this->_request = request;
	this->arg = absPath;
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
		setenv("CONTENT_LENGTH","178", 1);
		setenv("SERVER_PROTOCOL", _request.getProtocol().c_str(), 1);
		setenv("QUERY_STRING", _request.getQuery().c_str(), 1);
		setenv("REQUEST_METHOD",_request.getMethod().c_str(), 1);
		setenv("REDIRECT_STATUS","200",1);
		setenv("CONTENT_TYPE", "application/x-www-form-urlencoded",1);
		setenv("SCRIPT_FILENAME", arg.c_str(), 1);
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

void cgi::exec_script(int fd)
{
    int ret;
    char *tmp[3];
	int fd1;

	tmp[0] = (char*)scriptType.c_str();
	tmp[1] = (char*)arg.c_str();
	tmp[2] = NULL;

	setEnv();
	std::cout << "[ " << _request.getBodyFd() << " ]\n";
	fd1 = open(_request.getBodyFilename().c_str() , O_WRONLY | O_CREAT | O_TRUNC, 0777);
	dup2(fd1, 0);
	dup2(fd, 1);
    ret = execve(tmp[0], tmp, environ);
    if (ret == -1) {
		throw "500 Internal Server Error";
	}
}

void cgi::setHeader(const std::string &key, const std::string &value, bool end)
{
	this->_buffer += key + ": " + value + "\r\n";
	if (end)
		_buffer += "\r\n";
}

void cgi::handleResponse(int code)
{
	this->_buffer.insert(0, "Accept-Ranges: none\r\n");
	this->_buffer.insert(0, "Server: webserv/1.1 " + getOsName() + "\r\n");
	this->_buffer.insert(0, "Date: " + getDate() + "\r\n");
	this->_buffer.insert(0, "HTTP/1.1 " + std::to_string(code) + " " + getErrorMessage(200) + "\r\n");
	std::cout << _buffer;
}

void cgi::handleRedirectResponse()
{
	this->_buffer.insert(0, "Accept-Ranges: none\r\n");
	this->_buffer.insert(0, "Server: webserv/1.1 \r\n");
	this->_buffer.insert(0, "Date: " + getDate() + "\r\n");
	this->_buffer.insert(0, "HTTP/1.1 \r\n");
}

void cgi::processing_cgi(Request request, std::string absPath)
{
	int fd;
    pid_t pid;
	std::string filename = "response.txt";

	fd = open(filename.c_str() , O_WRONLY | O_CREAT | O_TRUNC, 0666);
	setRequest(request, absPath);
	std::cout << "[ -------- ] : " << _request.getQuery().c_str() << std::endl;
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	else if (pid == 0)
		exec_script(fd);
	_parseOutput(fd);
	close(fd);
	wait(NULL);
}
