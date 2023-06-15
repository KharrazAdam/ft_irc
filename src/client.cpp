/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 08:38:15 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/15 11:43:36 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

bool	is_alpha_valid(std::string& str)
{
	int	size = str.size();

	for (int i = 0; i < size; i++)
		if (std::isalpha(str[i]) == false && str[i] != ' ')
			return false;
	return true;
}

/**
 * @name Deafault constructor
 * @brief set auth to false; auth will be changed to 'true' when client will insert correct password
*/
client::client(int fds):fd(fds), auth(0), username(""), nickname(""), addr("nickname!user@host"){}
client::client():auth(0), username(""), nickname(""), addr("nickname!user@host"){}

client::~client(){}

/**
 * @brief change auth to 'status';
*/
void	client::SetAuth(int status)
{
	this->auth |= status;
}

/**
 * @brief set nickname with the given string
*/
void	client::SetNick(std::string str)
{
	this->nickname = str;
}

/**
 * @brief set username with the given string
*/
void	client::SetUser(std::string str)
{
	this->username = str;
}

/**
 * @brief return the auth (either true or false)
*/
int	client::ShowAuth(void)
{
	return auth;
}

std::string	client::getNick(void)
{
	return nickname;
}

int	client::getFd(void)
{
	return fd;
}

// COMMANDS
/**
 * @note please recode this function in a clean way
*/
bool	client::cmd_PASS(std::deque<std::string>& deq, std::string& pass)
{
	if (auth & PASSWORD)
		return send_error("ERR_ALREADYREGISTERED"), true; // ERR_ALREADYREGISTERED
	if (deq.size() == 1)
		return send_error("ERR_NEEDMOREPARAMS"), false; // ERR_NEEDMOREPARAMS
	deq.pop_front();
	if (deq.size() > 1 || deq.front().compare(pass) != 0)
		return send_error("ERR_PASSWDMISMATCH"), false; // ERR_PASSWDMISMATCH
	return auth |= PASSWORD, true;
}

/**
 * @note please recode this function in a clean way
*/
bool	client::cmd_NICK(std::deque<std::string>& deq, std::map<int, client>& cl)
{
	std::map<int, client>::iterator it = cl.begin();
	std::string	old(nickname);

	if (deq.size() == 1)
		return send_error("ERR_NONICKNAMEGIVEN"), false; // ERR_NONICKNAMEGIVEN
	deq.pop_front();
	if (deq.size() > 1 || deq.front().size() < 3 || deq.front()[0] == '#' || deq.front()[0] == ':' || deq.front().substr(0, 2) == "#&" || deq.front().substr(0, 2) == "&#")
		return send_error("ERR_ERRONEUSNICKNAME"), false; // ERR_ERRONEUSNICKNAME
	while (it != cl.end())
	{
		if ((*it).second.nickname == deq.front())
			return send_message("ERR_NICKNAMEINUSE"), false; // ERR_NICKNAMEINUSE	
		it++;
	}
	nickname = deq.front();
	auth |= NICKNAME;
	if ((auth & USERNAME))
	{
		auth |= AUTHENTIFICATED;
		RPL_WELCOME(); // send welcome
	}
	return send_message("NICK NAME DONE"), true;
}

bool	client::cmd_USER(std::deque<std::string>& deq)
{
	if (auth & AUTHENTIFICATED)
		return send_error("ERR_ALREADYREGISTRED"), false; // ERR_ALREADYREGISTRED
	if (deq.size() < 5)
		return send_error("ERR_NEEDMOREPARAMS"), false; // ERR_NEEDMOREPARAMS
	if (deq[4][0] != ':')
		return send_error("ERROR SYNTAX"), false; // ERR_chars
	deq.pop_front();
	username = deq.front();
	for (size_t i = 0; i < 3; i++)
		deq.pop_front();
	deq.front().erase(deq.front().begin());
	while (1)
	{
		realname += deq.front();
		deq.pop_front();
		if (deq.empty() == true)
			break ;
		realname += " ";
	}
	auth |= USERNAME;
	if ((auth & NICKNAME))
	{
		auth |= AUTHENTIFICATED;
		RPL_WELCOME(); // send welcome
	}
	return true;
}


void	client::send_message(const char* er) const
{
		std::string str;

	str.clear();
	str.append("MESSAGE ") += nickname + " " + er;
	if (send(fd, str.append("\n").c_str(), str.size() + 1, 0) == -1)
		send_message(er);
}

void	client::send_error(const char* er) const
{
	std::string str;

	str.clear();
	str.append("Error ") += nickname + ": " + er;
	if (send(fd, str.append("\n").c_str(), str.size() + 1, 0) == -1)
		send_error(er);
}

void	client::RPL_WELCOME(void)
{
	send(fd, "<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]\n",72, 0);
}

void	client::com_sep(std::deque<std::string>& deq, std::vector<std::string>& vec)
{
	deq.pop_front();
	if (deq.empty())
		return ;
	std::stringstream	ss(deq.front());
    std::string			str;
	while (std::getline(ss, str, ','))
        vec.push_back(str);
}

bool	client::cmd_JOIN(std::deque<std::string>& deq, std::map<std::string, Channel>& channels)
{
	std::vector<std::string>	titles;
	std::vector<std::string>	keys;

	if (deq.size() == 1)
		return send_error("ERR_NEEDMOREPARAMS"), false; // ERR_NEEDMOREPARAMS
	com_sep(deq, titles);
	com_sep(deq, keys);
	for (size_t i = keys.size(); i < titles.size(); i++)
		keys.push_back("");
	for (size_t i = 0; i < titles.size(); i++)
	{
		if (channels.find(titles[i]) == channels.end())
			channels[titles[i]] = ::Channel(titles[i], keys[i]); // channel doesn't exist
		if (!channels[titles[i]].GetKey().compare("") || !channels[titles[i]].GetKey().compare(keys[i]))
			channels[titles[i]].add_user(*this);
		else	
			send_error("ERR_BADCHANNELKEY");
	}
	return true;
}

// bool	client::sendUser(std::string& message, std::string& receiver, std::map<int, client>& cl)
// {
// 	std::map<int, client>::iterator it;
	
// 	for (it = cl.begin(); it != cl.end(); it++)
// 	{
// 		if ((*it).second.getNick() == receiver)
// 			break ;
// 	}
// 	if (it == cl.end())
// 		send_error("ERR_NOSUCHNICK");
// 	else
// 	{
// 		if (send((*it).second.getFd(), message.c_str(), message.size(), 0) == -1)
// 			send_error("send syscall error!");
// 	}
// 	return true;	
// }

bool	client::cmd_PRIVMSG(std::deque<std::string>& deq, std::map<int, client>& cl, std::map<std::string, Channel>& ch)
{
	std::vector<std::string>	receivers;
	std::string	message("");

	std::map<int, client>::iterator it;
	// std::map<std::string, Channel>::iterator it2;

	if (deq.size() < 3)
		return send_error("ERR_NOTEXTTOSEND"), false; // ERR_NOTEXTTOSEND
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
		{
			if (ch.find(receivers[i]) == ch.end())
				send_error("ERR_NOSUCHCHANNEL");
			else
			{
				for (size_t i = 0; i < ch[receivers[i]].users.size(); i++)
					if (send(ch[receivers[i]].users[i].getFd(), message.c_str(), message.size(), 0) == -1)
						send_error("ERR_SYSCALL_SEND");
			}
		}
		else  // users
		{
			for (it = cl.begin(); it != cl.end(); it++) 
			{
				if (receivers[i] == (*it).second.getNick())
					break ;
			}
			if (it == cl.end())
				send_error("ERR_NOSUCHNICK");
			else if (send((*it).second.getFd(), message.c_str(), message.size(), 0) == -1)
				send_error("ERR_SYSCALL_SEND");
		}
	}
	(void)ch;
	return true;
}
