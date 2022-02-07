/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 14:53:26 by aabounak          #+#    #+#             */
/*   Updated: 2022/02/07 16:07:49 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "LocationClass.hpp"

# define __AUTOINDEX_OFF__ 0
# define __AUTOINDEX_ON__ 1

class ServerConfig {
    /* ----- PRIVATE ----- */
    public:
        __SIZE_TYPE__   __port;
        std::string     __serverName;
        std::string     __host;
        __SIZE_TYPE__   __bodySizeLimit;
        std::string     __accessLog;
        std::string     __errorPage;
        bool            __autoindex;
        __SIZE_TYPE__   __locationCount;
        LocationClass * __location;
        
    public:
        /* ----- Constructors & Destructor respectively ----- */
        ServerConfig();
        ServerConfig( ServerConfig const &x );
        ServerConfig& operator= ( const ServerConfig& rhs );
        ~ServerConfig() ;

        /* ----- Getters ----- */
        __SIZE_TYPE__   getPort() const;
        std::string     getServerName() const;
        std::string     getHost() const;
        __SIZE_TYPE__   getBodySizeLimit() const;
        std::string     getAccessLog() const;
        std::string     getErrorPage() const;
        bool            getAutoIndex() const;
        __SIZE_TYPE__   getLocationCount() const;

        
};