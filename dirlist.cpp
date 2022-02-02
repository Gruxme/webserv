/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dirlist.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 16:32:45 by abiari            #+#    #+#             */
/*   Updated: 2022/02/02 18:54:47 by abiari           ###   ########.fr       */
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
	DIR *dir, *dirCheck;
	struct dirent	*ent;
	struct stat		*status = (struct stat *)malloc(sizeof(struct stat));
	std::string		path = "/Users/abiari/Desktop/test"; // to be replaced with URI
	char	date[15];
	std::ostringstream dirListHtml;
	dirListHtml << "<html>\n<head><title>Index of " << path << "</title></head>\n<body>\n<h1>Index of " << path << "</h1>\n<hr><pre><a href=\"../\">../</a>\n";
	if ((dir = opendir(path.c_str())) != NULL){
		while ((ent = readdir(dir)) != NULL){
			std::string	fileName(ent->d_name);
			if(fileName == ".")
				continue;
			dirListHtml << "<a href=\"" << fileName << "\">" << fileName << "</a>";
			stat(fileName.c_str(), status);
			strftime(date, 15, "%d-%b-%y %H:%M", gmtime(&(status->st_ctime)));
			dirListHtml << std::setw(60) << std::right << date;
			if((dirCheck = opendir(fileName.c_str())) == NULL && errno == ENOTDIR){
				closedir(dirCheck);
				dirListHtml << "-\n";
			}
			else{
				dirListHtml << std::setw(20) << std::right << status->st_size << std::endl;
			}
		}
		dirListHtml << "</pre><hr></body>\n</html>";
		std::cout << dirListHtml.str() << std::endl;
		closedir(dir);
	}
	else{
		/* could not open directory */
		perror("");
		return EXIT_FAILURE;
	}
}
