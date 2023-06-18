/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 08:38:15 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/18 00:24:23 by akharraz         ###   ########.fr       */
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

std::map<int, client>::iterator	client::mapFind(std::map<int, client>&map, std::string& nick)
{
	std::map<int, client>::iterator it;

    for (it = map.begin(); it != map.end(); it++)
	{
		if ((*it).second.getNick() == nick)
			break ;
	}
	return it;
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
	str += er;
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

bool client::cmd_KICK(std::deque<std::string>& deq, std::map<std::string,Channel>& channels)
{
	std::string					chan;
	std::vector<std::string>	nicks;
	Channel						*channel;

	if (deq.size() < 3)
		return send_error("ERR_NEEDMOREPARAMS"), false; // ERR_NEEDMOREPARAMS
	chan = deq[1];
	deq.pop_front();
	com_sep(deq, nicks);
	if (channels.find(chan) == channels.end())
		return send_error("ERR_NOSUCHCHANNEL"), false; // ERR_NOSUCHCHANNEL
	channel = &channels[chan];
	// std::vector<int>	&mods = channel->getMods();
	if (channel->vecFind(channel->mods, *this) == channel->mods.end())
		return send_error("ERR_CHANOPRIVSNEEDED"), false; // ERR_CHANOPRIVSNEEDED
	for (size_t i = 0; i < nicks.size(); i++)
	{
		// TODO: send kick message with comment message
		if (channel->kickUser(nicks[i], *this))
			continue;

	}
	if (channel->users.empty())
		channels.erase(chan);
	return true;
}

bool client::cmd_TOPIC(std::deque<std::string> & deq, std::map<std::string, Channel> & channels)
{
	std::string	chan;
	std::string	topic;

    if (deq.size() < 2)
		return send_error("ERR_NEEDMOREPARAMS"), false; // ERR_NEEDMOREPARAMS
	chan = deq[1];
	topic = deq[2];
	if (channels.find(chan) == channels.end())
		return send_error("ERR_NOSUCHCHANNEL"), false; // ERR_NOSUCHCHANNEL
	if (channels[chan].vecFind(channels[chan].users, *this) == channels[chan].users.end())
		return send_error("ERR_NOTONCHANNEL"), false; // ERR_NOTONCHANNEL
	if (channels[chan].isActive('t') && !channels[chan].isMod(*this))
		return send_error("ERR_CHANOPRIVSNEEDED"), false; // ERR_CHANOPRIVSNEEDED
	channels[chan].setTopic(topic, *this);
	return true;
}

bool client::cmd_INVITE(std::deque<std::string> & deq, std::map<int, client>& users, std::map<std::string, Channel> & channels)
{
	std::string	chan;
	std::string	nick;
	if (deq.size() < 3)
		return send_error("ERR_NEEDMOREPARAMS"), false; // ERR_NEEDMOREPARAMS
	chan = deq[1];
	nick = deq[2];
	cout << "salma : " << nick << endl;
	cout << "chan : " << chan << endl;
	if (channels.find(chan) == channels.end())
		return send_error("ERR_NOSUCHCHANNEL"), false; // ERR_NOSUCHCHANNEL
	if (channels[chan].vecFind(channels[chan].users, *this) == channels[chan].users.end())
		return send_error("ERR_NOTONCHANNEL"), false; // ERR_NOTONCHANNEL
	if (channels[chan].isActive('i') && !channels[chan].isMod(*this))
		return send_error("ERR_CHANOPRIVSNEEDED"), false; // ERR_CHANOPRIVSNEEDED
	if (channels[chan].vecFind(channels[chan].users, nick) != channels[chan].users.end())
		return send_error("ERR_USERONCHANNEL"), false; // ERR_USERONCHANNEL
	// if (channels[chan].vecFind(channels[chan].users, nick) != channels[chan].users.end())
	if (mapFind(users, nick) != users.end())
		channels[chan].inviteUser((*mapFind(users, nick)).second);
	else
		return send_error("ERR_NOSUCHNICK"), false; // ERR_NOSUCHNICK	
	return true;
}

