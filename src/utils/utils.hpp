/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 21:37:01 by aabounak          #+#    #+#             */
/*   Updated: 2022/03/12 21:57:34 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <iostream>
# include <string>
# include <vector>
# include <unistd.h>
# include <map>
# include <algorithm>
# include <sstream>
# include <fstream>

std::vector<std::string> _split( std::string str, char separator );
void    _eraseSubstr( std::string &str, const std::string &substr );
void    _eraseAllSubstr( std::string &str, const std::string &substr );
std::string _ltrim( const std::string &s, const std::string set );
std::string _rtrim(const std::string &s, const std::string set );
std::string _trim(const std::string &s, const std::string set );
bool    _checkHeadersKeySyntax( std::string key );
bool    _hasEnding( std::string const &fullString, std::string const &ending );
int     _findFileSize( int fd );
bool    _isHexNotation( std::string const& s );
int     _hexadecimalToDecimal( std::string hexVal );