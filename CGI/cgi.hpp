/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 13:32:54 by sel-fadi          #+#    #+#             */
/*   Updated: 2022/03/10 22:13:00 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <map>
#include <cstdlib>
#include <vector>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <istream>
#include <sys/types.h>
#include <signal.h>
#include <string>
#include "../src/request/Request.hpp"
// #include "../src/response/response.hpp"
// #include "../src/config/LocationClass.hpp"

class cgi
{
	public:
		Request _request;
		bool getOrPost;
		bool documentOrRedirection;
		std::string queryString;
		std::string arg;
		std::string scriptType;
		std::vector<std::string> my_headers;
		std::vector<std::string> my_headerPost;
		std::string _body;
		std::string _buffer;

	public:
		cgi();
		~cgi();
		cgi( cgi const &obj );
		cgi& operator=( cgi const &rhs );

		void processing_cgi(Request request, std::string absPath);
		void exec_script(int fd);
		// void script_output(int fd);
		
		std::string getDate();
		const char* getErrorMessage(int error);
		std::string getOsName();

		void setRequest(Request request, std::string absPath);

		std::string	_generateTmp( int fd ) {
			std::string tmp = "";
			/* NO NEED TO OPEN THE FILE DESCRIPTOR */
			struct pollfd  fds = {};
			char buffer[4096];
			bzero(&buffer, 4096);
			int count = 0;
			fds.fd = fd;
			fds.events = POLLIN;
			int rc = poll(&fds, 1, 0);
			if (rc == 1 && fds.events & POLLIN ) {				
				while ((count = read(fd, buffer, 4096)) > 0) {
					// count = read(fd, buffer, 4096);
					for (int i = 0; i < count; i++)
						tmp += buffer[i];
				}
			}
		}

	private:
		std::string	_status;
		std::string	_location;
		size_t		_contentLength;
		std::string	_output;

	# include <algorithm>

	public:
		void	parseOutput( int fd ) {
			std::string tmp = _generateTmp(fd);
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
			size_t pos = tmp.find("\r\n\r\n");
			if (pos != std::string::npos) {
				ss.seekg(pos, std::ios::end);
				_contentLength = ss.tellg();
			}
			_output += "Content-Length: ";
			_output += _contentLength;
			_output += "\r\nConnection: " + _request.getHeaders().find("Connection")->second;
			_output.append(tmp);
		}
			
		
		std::vector<std::string> setEnvInVector(Request &request);
		std::vector<std::string> setEnvInVectorPost(Request &request);
		char *const* setEnv(std::vector<std::string> my_env);
		void setEnv();
		void setHeader(const std::string &key, const std::string &value, bool end);
		void handleRedirectResponse();
		void handleResponse(int code);
};
