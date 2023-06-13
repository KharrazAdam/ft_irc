/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 17:11:18 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/13 21:06:31 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

Channel::Channel():title(""), key(""), topic(""), l(-1), i(false){};
Channel::Channel(std::string T, std::string K):title(T), key(K), topic(""), l(-1), k(false), i(false)
{
	if (key.compare("") != 0)
		k = true;
}

Channel::~Channel()
{}

std::string&	Channel::GetKey(void)
{
	return key;
}

bool	Channel::add_user(client& cl)
{
	if (users.find(cl.getNick()) != users.end())
		return true;
	if (users.size() == this->l)
			return (cl.send_error("ERR_CHANNELISFULL"), false);
	if (this->i == true) // check the invite list
	{
		if (invited.find(cl.getNick()) == invited.end())
			return (cl.send_error("ERR_INVITEONLYCHAN"), false);
		invited.erase(cl.getNick());
	}
	users.insert(std::make_pair<std::string, int>(cl.getNick(), cl.getFd()));
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
