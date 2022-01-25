/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 11:51:00 by aabounak          #+#    #+#             */
/*   Updated: 2022/01/25 05:00:21 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "temporaryHeader.hpp"

// RequestParser
class RequestParser
{
    /* private */
    public:
        std::vector<std::string> __req;
        std::string __method;
        std::string __route;
        std::string __protocol;
        std::map<std::string, std::string>  __headers;

    public:
        RequestParser() {}
        ~RequestParser() {}

        /* 
                DataS : ( You should be able to send me a void * and append it to the DS )
            For body -> if POST request (store body in file)
        */

        /* 
            1 - You will give me a buffer void *
            Had lbuffer append it to the data struct
            Buffer will have chunks from request.
            
            2 - Make a method isComplete() that will tell that the request dazet kamla
                
            3 - 
        
        */
        void    append( std::string buf ) {
            return ;
        }

        void    split( std::string str, char separator ) {
            int currentIndex = 0, i = 0, startIndex = 0, endIndex = 0;
            while (i < str.length()) {
                if (str[i] == separator || i == str.length()) {
                    endIndex = i;
                    std::string subString = "";
                    subString.append(str, startIndex, endIndex - startIndex);
                    __req.push_back(subString);
                    currentIndex++;
                    startIndex = endIndex + 1;
                }
                i++;
            }
            std::vector<std::string>::iterator it = __req.begin();
            for (; it != __req.end(); ++it) {
                std::cout << *it << it->length() << std::endl;
            }
        }

        

        
};
