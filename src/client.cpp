/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 08:38:15 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/09 20:11:03 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

/**
 * @name Deafault constructor
 * @brief set auth to false; auth will be changed to 'true' when client will insert correct password
*/
client::client(int fds):fd(fds), auth(false), username("unknown"), nickname("unknown"), addr("nickname!user@host"){}
client::client():auth(false), username("unknown"), nickname("unknown"), addr("nickname!user@host"){}

client::~client(){}

/**
 * @brief change auth to 'status';
*/
void	client::SetAuth(bool status)
{
	this->auth = status;
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
bool	client::ShowAuth(void) const
{
	return this->auth;
}

std::string&	client::getNick(void)
{
	return nickname;
}

// COMMANDS
/**
 * @note please recode this function in a clean way
*/
bool	client::cmd_PASS(std::deque<std::string>& deq, std::string& pass)
{
	if (ShowAuth() == true)
		return send_error("ERR_ALREADYREGISTERED"), true; // ERR_ALREADYREGISTERED
	if (deq.size() == 1)
		return send_error("ERR_NEEDMOREPARAMS"), false; // ERR_NEEDMOREPARAMS
	deq.pop_front();
	if (deq.size() > 1 || deq.front().compare(pass) != 0)
		return send_error("ERR_PASSWDMISMATCH"), false; // ERR_PASSWDMISMATCH
	return this->SetAuth(true), true;
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
	if (deq.size() > 1 || deq.front()[0] == '#' || deq.front()[0] == ':' || deq.front().substr(0, 2) == "#&" || deq.front().substr(0, 2) == "&#")
		return send_error("ERR_ERRONEUSNICKNAME"), false; // ERR_ERRONEUSNICKNAME
	if (deq.front().size() < 3)
		return send_error("ERR_ERRONEUSNICKNAME"), false;
	while (it != cl.end())
	{
		if ((*it).second.nickname == deq.front())
			return send_error("ERR_NICKNAMEINUSE"), false; // ERR_NICKNAMEINUSE	
		it++;
	}
	nickname = deq.front();
	return send_error("NICK NAME DONE"), true;
}

bool	client::cmd_USER(std::deque<std::string>& a)
{
	(void)a;
	return true;
}


void	client::send_message(const char* er) const
{
		std::string str;

	str.clear();
	str.append("MESSAGE ") += er;
	if (send(fd, str.append("\n").c_str(), str.size() + 1, 0) == -1)
		send_message(er);
}

void	client::send_error(const char* er) const
{
	std::string str;

	str.clear();
	str.append("Error ") += er;
	if (send(fd, str.append("\n").c_str(), str.size() + 1, 0) == -1)
		send_error(er);
}
