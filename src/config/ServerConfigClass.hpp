/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigClass.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 14:53:26 by aabounak          #+#    #+#             */
/*   Updated: 2022/02/23 11:05:02 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "LocationClass.hpp"

class ServerConfigClass {

    friend class ConfigClass;

    private:
        size_t			_port;
        std::string     _serverName;
        std::string     _root;
        std::string     _redirect;
        std::string     _index;
        size_t          _bodySizeLimit;
        std::string     _accessLog;
        std::string     _errorPage;
        size_t			_locationCount;
        std::vector<LocationClass> _location;

    public:
        ServerConfigClass();
        ServerConfigClass( ServerConfigClass const &x );
        ServerConfigClass& operator=( ServerConfigClass const& rhs );
        ~ServerConfigClass() ;
        
        size_t			getPort() const;
        std::string     getServerName() const;
        std::string     getRoot() const;
        std::string     getRedirect() const;
        std::string     getIndex() const;
        size_t			getBodySizeLimit() const;
        std::string     getAccessLog() const;
        std::string     getErrorPage() const;
        size_t			getLocationCount() const;
        std::vector<LocationClass>  getLocationClass() const;

};
