/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 08:38:15 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/08 20:38:09 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

/**
 * @name Deafault constructor
 * @brief set auth to false; auth will be changed to 'true' when client will insert correct password
*/
client::client(int fds):fd(fds), auth(false), username("unknown"), nickname("unknown"){}
client::client():auth(false), username("unknown"), nickname("unknown"){}

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
bool	client::cmd_PASS(std::deque<std::string>& deq, std::string& pass)
{
	if (ShowAuth() == true)
		return ERR_REPETETIVE(" :You may not reregister\n"), true; // ERR_ALREADYREGISTERED
	if (deq.size() == 1)
		return ERR_NEEDMOREPARAMS("PASS"), false; // ERR_NEEDMOREPARAMS
	deq.pop_front();
	if (deq.size() > 1 || deq.front().compare(pass) != 0)
		return ERR_REPETETIVE(" :Password incorrect\n"), false; // ERR_PASSWDMISMATCH
	return this->SetAuth(true), true;
}

bool	client::cmd_NICK(std::deque<std::string>& deq)
{
	if (deq.size() == 1)
		return ERR_REPETETIVE(" :No nickname given\n"), false; // ERR_NONICKNAMEGIVEN
	
	deq.pop_front();
	return this->SetAuth(true), true;
}


// ERRORS

void	client::ERR_NEEDMOREPARAMS(const char *cmd)
{
	std::string str;

	str.clear();
	str = nickname + " " + cmd + " :Not enough parameters\n";
	send(fd, str.c_str(), str.size(), 0);
}

void	client::ERR_REPETETIVE(const char* msg)
{
	std::string str;

	str.clear();
	str = nickname + msg;
	send(fd, str.c_str(), str.size(), 0);
}
