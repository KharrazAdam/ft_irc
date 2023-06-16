/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 08:19:02 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/16 08:21:44 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

bool	client::noticeCl(std::map<int, client>& cl, std::string& message, std::string& receiver)
{
	std::map<int, client>::iterator it;

	for (it = cl.begin(); it != cl.end(); it++)
	{
		if (receiver == (*it).second.getNick())
			break ;
	}
	if (it == cl.end())
		return false;
	else if (send((*it).second.getFd(), message.c_str(), message.size(), 0) == -1)
		return false;
	return true;
}

bool	client::noticeCh(std::map<std::string, Channel>& ch, std::string& message, std::string& receiver)
{
	if (ch.find(receiver) == ch.end())
		return false;
	else
	{
		if (ch[receiver].vecFind(ch[receiver].users, *this) == ch[receiver].users.end()) /// not a cmember
			return false;
		for (size_t j = 0; j < ch[receiver].users.size(); j++)
			send(ch[receiver].users[j].getFd(), message.c_str(), message.size(), 0);
	}
	return true;
}

bool	client::cmd_NOTICE(std::deque<std::string>& deq, std::map<int, client>& cl, std::map<std::string, Channel>& ch)
{
	std::vector<std::string>	receivers;
	std::string	message("");


	if (deq.size() < 3)
		return false;
	com_sep(deq, receivers);
	deq.pop_front();
	while (true)
	{
		message.append(deq.front());
		deq.pop_front();
		if (deq.empty() == true)
		{	
			message.append("\n");
			break ;
		}
		message.append(" ");
	}
	for (size_t i = 0; i < receivers.size(); i++)
	{
		if (receivers[i][0] == '#')
			msgCh(ch, message, receivers[i]);
		else
			msgCl(cl, message, receivers[i]);
	}
	return true;
}
