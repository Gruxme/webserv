/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <aabounak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 11:14:05 by abiari            #+#    #+#             */
/*   Updated: 2022/02/14 15:53:28 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

class response {
	public:
		response();
		response(const response &x);
		~response();
		response&	operator=(const response &x);

		void	autoindexModule(void);
	private:

};


#endif //WEBSERV_RESPONSE_HPP
