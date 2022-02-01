/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dirlist.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 16:32:45 by abiari            #+#    #+#             */
/*   Updated: 2022/02/01 19:05:58 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <iostream>

int main(void)
{
	DIR *dir;
	struct dirent	*ent;
	struct stat		*status = (struct stat *)malloc(sizeof(struct stat));
	char*	path = getcwd(NULL, 0);
	char	date[15];
	if ((dir = opendir(path)) != NULL){
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL){
			std::string	fileName(ent->d_name);
			if(fileName == ".")
				continue;
			stat(fileName.c_str(), status);
			strftime(date, 15, "%d-%m-%y %H:%M", gmtime(&(status->st_ctime)));
			std::cout << fileName << "		" << date << "		" << status->st_size << std::endl;
			// printf("%s size: %lld date modified: %s\n", ent->d_name, status->st_size, date);
		}
		closedir(dir);
	}
	else{
		/* could not open directory */
		perror("");
		return EXIT_FAILURE;
	}
}
