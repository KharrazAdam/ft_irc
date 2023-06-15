/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 17:11:18 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/15 08:35:00 by akharraz         ###   ########.fr       */
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

std::string&	Channel::getTitle(void)
{
	return title;
}

std::vector<client>::iterator	Channel::vecFind(std::vector<client>& vec, client& cl)
{
	std::vector<client>::iterator it;

    for (it = vec.begin(); it != vec.end(); it++)
	{
		if ((*it).getNick() == cl.getNick())
			break ;
	}
	return it;
}

bool	Channel::add_user(client &cl)
{   
	std::vector<client>::iterator	it;

	if (vecFind(users, cl) != users.end())
		return true;
	if (l > 0 && users.size() == (size_t)this->l)
			return (cl.send_error("ERR_CHANNELISFULL"), false);
	if (this->i == true) // check the invite list
	{
		if ((it = vecFind(invited, cl)) == invited.end())
			return (cl.send_error("ERR_INVITEONLYCHAN"), false);
		invited.erase(it);
	}
	users.push_back(cl);
	return true;
}

void	Channel::show_details()
{
	std::vector<client>::iterator it = users.begin();

	std::cout << "title: " << title << std::endl;
	std::cout << "key: " << key << std::endl; 
	std::cout << "users: "; 
	for (it = users.begin(); it != users.end(); it++)
		std::cout << (*it).getNick() << " ";
	std::cout << std::endl;
	return ;
}
