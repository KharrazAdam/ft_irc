/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 17:11:18 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/17 21:26:04 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

Channel::Channel():title(""), key(""), topic(""), l(-1), i(false), t(false){};
Channel::Channel(std::string T, std::string K):title(T), key(K), topic(""), l(-1), k(false), i(false), t(false)
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

void	Channel::set_i(bool x)
{
	this->i = x;
}

void	Channel::set_t(bool x)
{
	this->t = x;
}

void	Channel::set_k(bool x)
{
	this->k = x;
}

void	Channel::setKey(std::string str)
{
	key = str;
}

std::vector<client>::iterator	Channel::vecFind(std::vector<client>& vec, client& cl)
{
	std::vector<client>::iterator it;

    for (it = vec.begin(); it != vec.end(); it++)
	{
		if ((*it).getNick() == cl.getNick())
			break ;
	}
	if (it != vec.end())
		std::cout << (*it).getNick() << std::endl;
	return it;
}

bool	Channel::add_user(client &cl)
{   
	std::vector<client>::iterator	it;

	if (vecFind(users, cl) != users.end())
		return true;
	if (l > 0 && users.size() == (size_t)this->l)
			return (cl.send_error("ERR_CHANNELISFULL"), false);
	if (this->i == true)
	{
		if ((it = vecFind(invited, cl)) == invited.end())
			return (cl.send_error("ERR_INVITEONLYCHAN"), false);
		invited.erase(it);
	}
	users.push_back(cl);
	return true;
}
