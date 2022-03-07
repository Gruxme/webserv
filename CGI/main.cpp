/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-fadi <sel-fadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 12:35:15 by sel-fadi          #+#    #+#             */
/*   Updated: 2022/03/06 11:36:10 by sel-fadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

int main()
{
    cgi my_cgi;
    Request my_request;
    // std::cout << my_request.getUri() << "\n";
    my_cgi.processing_cgi(my_request);
    return 0;
}