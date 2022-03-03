/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationClass.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 01:50:10 by aabounak          #+#    #+#             */
/*   Updated: 2022/03/03 11:35:49 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <string>
# include <vector>

# define _AUTOINDEX_OFF_ 0
# define _AUTOINDEX_ON_ 1

class LocationClass {
    private:
        std::string _path;
        std::string _root;
        std::string _method;
        std::string _redirect;
        std::string _cgiExt;
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
        
        /* -- PUBLIC METHOD */
        std::vector<std::string> split( std::string str, char separator );

        /* ----- Getters----- */
        std::string getPath( void ) const;
        std::string getRoot( void ) const;
        std::string getMethod( void ) const;
        std::string getRedirect( void ) const;
        std::string getCgiExt( void ) const;
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