/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 08:38:15 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/06 09:01:16 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

/**
 * @name Deafault constructor
 * @brief set auth to false; auth will be changed to 'true' when client will insert correct password
*/
client::client():auth(false), username("server"), nickname("server"){}

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
bool	client::GetAuth(void) const
{
	return this->auth;
}

