/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 17:11:18 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/13 18:10:23 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

Channel::Channel():title(""), key(""), num(-1)
{}

Channel::Channel(std::string T, std::string K):title(T), key(K), num(-1)
{}

Channel::~Channel()
{}

std::string&	Channel::GetKey(void)
{
	return key;
}

bool	Channel::add_user(client& cl)
{
	if (users.find(cl.getNick()) == users.end())
		users.insert(std::make_pair<std::string, int>(cl.getNick(), cl.getFd()));
	(void)num;
	return true;
}

void	Channel::show_details()
{
	std::map<std::string, int>::iterator it = users.begin();

	std::cout << "title: " << title << std::endl; 
	std::cout << "key: " << key << std::endl; 
	std::cout << "users: "; 
	for (it = users.begin(); it != users.end(); it++)
		std::cout << (*it).first << " ";
	std::cout << std::endl;
}