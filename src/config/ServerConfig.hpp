/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 14:53:26 by aabounak          #+#    #+#             */
/*   Updated: 2022/02/22 14:00:17 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "LocationClass.hpp"

# define __AUTOINDEX_OFF__ 0
# define __AUTOINDEX_ON__ 1

class ServerConfig {
    /* ----- PRIVATE ----- */
    public:
        size_t			__port;
        std::string     __serverName;
        std::string     __root;
        size_t			__bodySizeLimit;
        std::string     __accessLog;
        std::string     __errorPage;
        bool            __autoindex;
        size_t			__locationCount;
        LocationClass * __location;
        
    public:
        /* ----- Constructors & Destructor respectively ----- */
        ServerConfig();
        ServerConfig( ServerConfig const &x );
        ServerConfig& operator= ( const ServerConfig& rhs );
        ~ServerConfig() ;

        /* ----- Getters ----- */
        size_t			getPort() const;
        std::string     getServerName() const;
        std::string     getRoot() const;
        size_t			getBodySizeLimit() const;
        std::string     getAccessLog() const;
        std::string     getErrorPage() const;
        bool            getAutoIndex() const;
        size_t			getLocationCount() const;

        
};
