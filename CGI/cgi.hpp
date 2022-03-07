/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-fadi <sel-fadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 13:32:54 by sel-fadi          #+#    #+#             */
/*   Updated: 2022/03/06 14:04:33 by sel-fadi         ###   ########.fr       */
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

class cgi
{
	public:
		Request request;
		bool getOrPost;
		bool documentOrRedirection;
		char *queryString;
		std::string arg;
		std::string scriptType;
		std::vector<std::string> my_headers;
		std::vector<std::string> my_headerPost;
		std::string _buffer;
	public:
		cgi();
		~cgi();
		cgi( cgi const &obj );
		cgi& operator= ( cgi const& rhs );

		void processing_cgi(Request &request);
		void exec_script(int *fd, int *fd1);
		void script_output(int *fd, int *fd1, pid_t pid);
		
		std::string getDate();
		const char* getErrorMessage(int error);
		std::string getOsName();
		
		std::vector<std::string> setEnvInVector(Request &request);
		std::vector<std::string> setEnvInVectorPost(Request &request);
		char *const* setEnv(std::vector<std::string> my_env);
		void setEnv(int getOrPost);
		void setHeader(const std::string &key, const std::string &value, bool end);
		void handleRedirectResponse();
		void handleResponse(int code);
};
