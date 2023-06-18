/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 05:54:56 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/18 04:33:06 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

bool	client::msgCl(std::map<int, client>& cl, std::string& message, std::string& receiver)
{
	std::map<int, client>::iterator it;

	for (it = cl.begin(); it != cl.end(); it++)
	{
		if (receiver == (*it).second.getNick())
			break ;
	}
	if (it == cl.end())
		return send_error("ERR_NOSUCHNICK"), false;
	else if (send((*it).second.getFd(), message.c_str(), message.size(), 0) == -1)
		return send_error("ERR_SYSCALL_SEND"), false;
	return true;
}

bool	client::msgCh(std::map<std::string, Channel>& ch, std::string& message, std::string& receiver)
{
	if (ch.find(receiver) == ch.end())
		return send_error("ERR_NOSUCHCHANNEL"), false;
	else
	{
		if (ch[receiver].vecFind(ch[receiver].users, *this) == ch[receiver].users.end()) /// not a cmember
			return send_error("ERR_CANNOTSENDTOCHAN"), false;
		for (size_t j = 0; j < ch[receiver].users.size(); j++)
		{
			if (send(ch[receiver].users[j]->getFd(), message.c_str(), message.size(), 0) == -1)
				send_error("ERR_SYSCALL_SEND");
		}
	}
	return true;
}

bool	client::cmd_PRIVMSG(std::deque<std::string>& deq, std::map<int, client>& cl, std::map<std::string, Channel>& ch)
{
	std::vector<std::string>	receivers;
	std::string	message("");


	if (deq.size() < 3)
		return send_error(":irc.example.com 433 * ben :Nickname is already in use"), false; // ERR_NOTEXTTOSEND
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
