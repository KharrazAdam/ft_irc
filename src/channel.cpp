/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzridi <mzridi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 17:11:18 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/15 19:49:54 by mzridi           ###   ########.fr       */
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
	if (this->l > 0 && users.size() == (size_t)this->l)
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

std::map<std::string, int> &Channel::getUsers(void)
{
	return users;
}

std::vector<int>& Channel::getMods(void)
{
	return mods;
}

bool Channel::kickUser(std::string &nick, client &cl)
{
	if (users.find(cl.getNick()) == users.end())
	{
		cl.send_error("ERR_NOTONCHANNEL");
		return false;
	}
	if (users.find(nick) == users.end())
	{
		std::cout << "nick: " << nick << std::endl;
		cl.send_error("ERR_USERNOTINCHANNEL");
		return false;
	}
	users.erase(nick);
	return true;
}

bool Channel::addMod(int fd)
{
	std::vector<int>::iterator it = mods.begin();

	for (it = mods.begin(); it != mods.end(); it++)
	{
		if (*it == fd)
			return false;
	}
	mods.push_back(fd);
	return true;
}

bool Channel::setTopic(std::string &topic, client &cl)
{
	if (users.find(cl.getNick()) == users.end())
	{
		cl.send_error("ERR_NOTONCHANNEL");
		return false;
	}
    if(t && std::find(mods.begin(), mods.end(), cl.getFd()) == mods.end())
	{
		cl.send_error("ERR_CHANOPRIVSNEEDED");
		return false;
	}
	this->topic = topic;
	return true;
}

bool Channel::isMod(int fd)
{
	std::vector<int>::iterator it = mods.begin();

	for (it = mods.begin(); it != mods.end(); it++)
		if (*it == fd)
			return true;
	return false;
}

bool Channel::isUser(std::string &nick)
{
    return users.find(nick) != users.end();
}

bool Channel::isActive(const char &mode)
{
	if (mode == 'i')
		return i;
    if (mode == 't')
		return t;
	if (mode == 'l')
		return l != -1;
	return true;
}

bool Channel::inviteUser(std::string &nick)
{
	invited.insert(nick);
	return true;
}