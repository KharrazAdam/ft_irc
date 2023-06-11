/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 08:38:15 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/11 02:47:36 by akharraz         ###   ########.fr       */
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
	return this->SetAuth(PASSWORD), true;
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
	if (!(auth & AUTHENTIFICATED))
		; // send welcome
	return send_message("NICK NAME DONE"), true;
}

bool	client::cmd_USER(std::deque<std::string>& deq)
{
	if (auth & AUTHENTIFICATED)
		return send_error("ERR_ALREADYREGISTRED"), false; // ERR_ALREADYREGISTRED
	if (deq.size() < 5)
		return send_error("ERR_NEEDMOREPARAMS"), false; // ERR_NEEDMOREPARAMS
	if (deq[2] != "0" || deq[3] != "*" || deq[4][0] != ':')
		return send_error("ERROR SYNTAX"), false; // ERR_chars
	deq.pop_front();
	username = deq.front();
	for (size_t i = 0; i < 3; i++)
		deq.pop_front();
	if (is_alpha_valid(username) == false)
		return send_error("ERROR NON VALID CHARS"), username.clear(), false; // ERR_chars
	deq.front().erase(deq.front().begin());
	while (1)
	{
		realname += deq.front();
		deq.pop_front();
		if (is_alpha_valid(realname) == false)
			return send_error("ERROR NON VALID CHARS"), realname.clear(), false; // ERR_chars
		if (deq.empty() == true)
			break ;
		realname += " ";
	}
	auth |= USERNAME;
	if (auth & NICKNAME)
		; // send welcome
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
	auth |= AUTHENTIFICATED;
	send(fd, "<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]\n",72, 0);
}