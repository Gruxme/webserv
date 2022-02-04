/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dirlist.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 16:32:45 by abiari            #+#    #+#             */
/*   Updated: 2022/02/03 16:22:02 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctime>
#include <sstream>
#include <sys/stat.h>
#include <iostream>
#include <iomanip>

int main(void)
{
	DIR *dir;
	struct dirent	*ent;
	struct stat		*status = new struct stat();
	std::string		path = "/Users/abiari/Desktop/test/"; // to be replaced with URI
	char			*date = new char[20]();
	std::ostringstream dirListHtml;
	dirListHtml << "<html>\n<head><title>Index of " << path << "</title></head>\n<body>\n<h1>Index of "\
				<< path << "</h1>\n<hr><pre><a href=\"../\">../</a>\n";
	if ((dir = opendir(path.c_str())) != NULL){
		while ((ent = readdir(dir)) != NULL){
			std::string	fileName(ent->d_name);
			if(fileName == "." || fileName == "..")
				continue;
			if (stat((path + fileName).c_str(), status) < 0)
				perror("stat: ");
			if(S_ISDIR(status->st_mode))
				fileName += "/";
			dirListHtml << "<a href=\"" << fileName << "\">" << fileName << "</a>";
			strftime(date, 17, "%d-%b-%y %H:%M", gmtime(&(status->st_mtimespec.tv_sec)));
			dirListHtml << std::setw(62 - fileName.length()) << date;
			if(S_ISDIR(status->st_mode))
				dirListHtml << std::setw(21) << std::right << "-\n";
			else{
				dirListHtml << std::setw(20) << status->st_size << std::endl;
			}
		}
		dirListHtml << "</pre><hr></body>\n</html>";
		std::cout << dirListHtml.str() << std::endl;
		closedir(dir);
	}
	else{
		/* could not open directory */
		if(errno == ENOENT)
			//send not found response
		else
			//send forbidden response
		return EXIT_FAILURE;
	}
}
