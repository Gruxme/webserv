/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-fadi <sel-fadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 13:32:54 by sel-fadi          #+#    #+#             */
/*   Updated: 2022/03/10 17:13:20 by sel-fadi         ###   ########.fr       */
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
		
		std::vector<std::string> setEnvInVector(Request &request);
		std::vector<std::string> setEnvInVectorPost(Request &request);
		char *const* setEnv(std::vector<std::string> my_env);
		void setEnv();
		void setHeader(const std::string &key, const std::string &value, bool end);
		void handleRedirectResponse();
		void handleResponse(int code);
};
