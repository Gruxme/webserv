/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationClass.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 01:50:10 by aabounak          #+#    #+#             */
/*   Updated: 2022/03/12 20:47:36 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <iostream>
# include <string>
# include <vector>

# define _AUTOINDEX_OFF_ 0
# define _AUTOINDEX_ON_ 1

class LocationClass {
    private:
        std::string _path;
        std::string _root;
        std::vector<std::string> _methods;
        std::string _redirect;
        std::string _upload;
        std::vector<std::string> _cgi;
        bool        _autoindex;
        
    public:
        /* ----- Constructors & Destructor respectively ----- */
        LocationClass();
        ~LocationClass();
        LocationClass( LocationClass const &x );
        LocationClass &operator=( LocationClass const &rhs );

        /* ----- PRIVATE ----- */
        /* ----- Location Parser ----- */
        void    parseLocation( std::string buffer );

        /* ----- Getters----- */
        std::string getPath( void ) const;
        std::string getRoot( void ) const;
        std::vector<std::string> getMethods( void ) const;
        std::string getRedirect( void ) const;
        std::string getUpload( void ) const;
        std::vector<std::string> getCgi( void ) const;
        bool        getAutoIndex( void ) const;

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

        class parseErr : public std::exception {
        public:
            explicit parseErr( const std::string &errStr ) throw() : _errStr(errStr) {}
            ~parseErr() throw() {}
            virtual const char * what() const throw() {
                return (_errStr.c_str());
            }
        private:
            std::string _errStr;
        };
};