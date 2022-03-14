/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationClass.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 01:50:10 by aabounak          #+#    #+#             */
/*   Updated: 2022/03/12 21:55:53 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <iostream>
# include <string>
# include <vector>
# include "../utils/utils.hpp"


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