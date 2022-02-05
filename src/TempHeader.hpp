/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TempHeader.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 01:07:32 by aabounak          #+#    #+#             */
/*   Updated: 2022/01/31 10:28:46 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# include <vector>
# include <map>
# include <list>

# include <sstream>
# include <iostream>
# include <fstream>
# include <vector>
# include <string>
# include <cstring>
# include <algorithm>

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

# include "main_parser/ConfigFileParser.hpp"
# include "main_parser/ServerConfig.hpp"
# include "main_parser/LocationClass.hpp"
# include "request_parser/Request.hpp"

# define PORT 8080
# define SERVER 1
# define USER_AGENT 0

