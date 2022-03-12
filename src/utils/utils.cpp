/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 21:37:02 by aabounak          #+#    #+#             */
/*   Updated: 2022/03/12 21:55:14 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "utils.hpp"

std::vector<std::string> _split( std::string str, char separator ) {
    std::vector<std::string>  myvec;
    size_t currentIndex = 0, i = 0, startIndex = 0, endIndex = 0;
    while (i <= str.length()) {
        if (str[i] == separator || i == str.length()) {
            endIndex = i;
            std::string subString = "";
            subString.append(str, startIndex, endIndex - startIndex);
            myvec.push_back(subString);
            currentIndex++;
            startIndex = endIndex + 1;
        }
        i++;
    }
    return myvec;
}

void    _eraseSubstr( std::string &str, const std::string &substr ) {
    size_t pos = str.find(substr);
    if (pos != std::string::npos)
        str.erase(pos, substr.length());
}

void    _eraseAllSubstr( std::string &str, const std::string &substr ) {
    size_t pos = std::string::npos;
    while ((pos = str.find(substr)) != std::string::npos)
        str.erase(pos, substr.length());
}

std::string _ltrim(const std::string &s, const std::string set ) {
    size_t start = s.find_first_not_of(set);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string _rtrim(const std::string &s, const std::string set ) {
    size_t end = s.find_last_not_of(set);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string _trim(const std::string &s, const std::string set ) {
    return _rtrim(_ltrim(s, set), set);
}

bool    _checkHeadersKeySyntax( std::string key ) {
    for (size_t i = 0; i < key.length(); i++) {
        if (!((key[i] >= 'a' && key[i] <= 'z') || (key[i] >= 'A' && key[i] <= 'Z') || key[i] == '-'))
            return false;
    }
    return true;
}

bool    _hasEnding( std::string const &fullString, std::string const &ending ) {
    if (fullString.length() >= ending.length()) { return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending)); }
    return false;
}

int     _findFileSize( int fd ) {
    off_t fsize;
    fsize = lseek(fd, 0, SEEK_END);
    return fsize;
}

bool    _isHexNotation( std::string const& s ) {
    return s.find_first_not_of("0123456789abcdefABCDEF\r\n", 2) == std::string::npos;
}

int     _hexadecimalToDecimal( std::string hexVal ) {
    int len = hexVal.size();
    int base = 1;
    int dec_val = 0;
    for (int i = len - 1; i >= 0; i--) {
        if (hexVal[i] >= '0' && hexVal[i] <= '9') {
            dec_val += (int(hexVal[i]) - 48) * base;
            base = base * 16;
        }
        else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {
            dec_val += (int(hexVal[i]) - 55) * base;
            base = base * 16;
        }
    }
    return dec_val;
}
