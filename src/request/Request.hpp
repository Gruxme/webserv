/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 10:30:00 by aabounak          #+#    #+#             */
/*   Updated: 2022/02/02 19:21:28 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <iostream>
# include <unistd.h>
# include <sstream>
# include <fstream>
# include <string>
# include <vector>
# include <map>

# define PY 1
# define PHP 2
# define __CONTENT_LENGTH_FOUND__ 0
# define __CONTENT_LENGTH_NOT_FOUND__ 1
# define __CONTENT_LENGTH_NEGATIVE__ 1
# define __BODY_COMPLETE__ 1
# define __BODY_INCOMPLETE__ 2

class Request {
    /* ----- PRIVATE ----- */
    private:
        std::string __dataGatherer;
        std::string __method;
        std::string __uri;
        std::string __protocol;
        short       __uriExtension;
        std::map<std::string, std::string>  __headers;
        std::string __bodyFilename;
		bool		__status;
        
    public:
        /* ----- Constructors & Destructor respectively ----- */
        Request();
        ~Request();

        std::string getDataGatherer( void ) const;
        std::string getMethod( void ) const;
        std::string getUri( void ) const;
        std::string getProtocol(void ) const;
        short       getUriExtension( void ) const;
        std::map<std::string, std::string> const &getHeaders( void ) const;
        std::string getBodyFilename( void ) const;
		bool		isComplete( void ) const;

        /* -- PUBLIC METHODS */
        void    append( const char * recvBuffer );

    private:
        /* PVT -- THESE SHOULD CHECK FOR STANDARDS LATER -- */
        void    __extractRequestLine( std::stringstream & iss );
        void    __extractHeaders( std::stringstream & iss );
        void    __handleChunkedRequest( std::stringstream & iss );
        void    __handleBasicRequest( std::stringstream & iss );
        // void    __extractContent( std::stringstream & iss );
		bool	__headersComplete( void );
        bool    __bodyComplete( void );


    public:
		void	parse( void );

    private:
        /* ----- Utils ------ */
        std::vector<std::string> __split( std::string str, char separator );
        void    __eraseSubstr( std::string &str, const std::string &substr );
        void    __eraseAllSubstr( std::string &str, const std::string &substr );
        std::string __ltrim( const std::string &s, const std::string &delim );
        bool    __hasEnding( std::string const &fullString, std::string const &ending );
        int     __findFileSize( std::ofstream &file );
        bool    __isHexNotation( std::string const& s );
        int     __hexadecimalToDecimal( std::string hexVal );
        bool    __checkContentLength( void );
        short   __compareContentLengthWithBody( std::ofstream &f );


        /* ----- Exceptions ----- */
        class parseErr : public std::exception {
        public:
            explicit parseErr(const std::string &errStr) throw() : _errStr(errStr) {}
            ~parseErr() throw() {}
            virtual const char * what() const throw() {
                return (_errStr.c_str());
            }
        private:
            std::string _errStr;
        };

};

std::ostream & operator<<( std::ostream & o, Request const & req );
