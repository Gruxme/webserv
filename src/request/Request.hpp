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
# include <algorithm>

# define PY 1
# define PHP 2
# define _CONTENT_LENGTH_FOUND_ 0
# define _CONTENT_LENGTH_NOT_FOUND_ 1
# define _CONTENT_LENGTH_NEGATIVE_ 1
# define _BODY_COMPLETE_ 1
# define _BODY_INCOMPLETE_ 2

class Request {
    /* ----- PRIVATE ----- */
    private:
        std::string _dataGatherer;
        std::string _method;
        std::string _uri;
        std::string _query;
        std::string _path;
        std::string _protocol;
        short       _uriExtension;
        std::map<std::string, std::string>  _headers;
		int			_port;
        std::string _bodyFilename;
		bool		_status;
        
    public:
        /* ----- Constructors & Destructor respectively ----- */
        Request();
        ~Request();
        Request( Request const &x );
        Request& operator= ( Request const& rhs );

        std::string getDataGatherer( void ) const;
        std::string getMethod( void ) const;
        std::string getUri( void ) const;
        std::string getQuery( void ) const;
        std::string getPath( void ) const;
        std::string getProtocol(void ) const;
        short       getUriExtension( void ) const;
        std::map<std::string, std::string> const &getHeaders( void ) const;
		int 		getPort( void ) const;
        std::string getBodyFilename( void ) const;
		bool		isComplete( void ) const;

        /* -- PUBLIC METHODS */
        void    append( const char * recvBuffer );

    private:
        /* PVT -- THESE SHOULD CHECK FOR STANDARDS LATER -- */
        void    _extractRequestLine( std::stringstream & iss );
        void    _extractHeaders( std::stringstream & iss );
        void    _handleChunkedRequest( std::stringstream & iss );
        void    _handleBasicRequest( std::stringstream & iss );
        // void    _extractContent( std::stringstream & iss );
		bool	_headersComplete( void );
        bool    _bodyComplete( void );


    public:
		void	parse( void );

    private:
        /* ----- Utils ------ */
        std::vector<std::string> _split( std::string str, char separator );
        void    _eraseSubstr( std::string &str, const std::string &substr );
        void    _eraseAllSubstr( std::string &str, const std::string &substr );
        std::string _ltrim( const std::string &s, const std::string &delim );
        bool    _hasEnding( std::string const &fullString, std::string const &ending );
        int     _findFileSize( std::ofstream &file );
        bool    _isHexNotation( std::string const& s );
        int     _hexadecimalToDecimal( std::string hexVal );
        bool    _checkContentLength( void );
        short   _compareContentLengthWithBody( std::ofstream &f );


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
