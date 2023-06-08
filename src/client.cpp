/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 08:38:15 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/08 17:47:23 by akharraz         ###   ########.fr       */
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


bool	client::cmd_PASS(std::deque<std::string>& deq, std::string& pass)
{
	if (ShowAuth() == true)
		return ERR_ALREADYREGISTERED(), true;
	if (deq.size() == 1)
		return ERR_NEEDMOREPARAMS("PASS"), false; // unsufisant params
	deq.pop_front();
	if (deq.size() > 1 || deq.front().compare(pass) != 0)
		return ERR_PASSWDMISMATCH(), false; // wrong password
	return this->SetAuth(true), true;
}

// COMMANDS

void	client::ERR_NEEDMOREPARAMS(const char *cmd)
{
	std::string str;

	str.clear();
	str = nickname + " " + cmd + " :Not enough parameters\n";
	send(fd, str.c_str(), str.size(), 0);
}

void	client::ERR_PASSWDMISMATCH(void)
{
	std::string str;

	str.clear();
	str = nickname + " :Password incorrect\n";
	send(fd, str.c_str(), str.size(), 0);
}

void	client::ERR_ALREADYREGISTERED(void)
{
	std::string str;

	str.clear();
	str = nickname + " :You may not reregister\n";
	send(fd, str.c_str(), str.size(), 0);
}