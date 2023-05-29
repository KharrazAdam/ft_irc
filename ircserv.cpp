/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 16:38:00 by akharraz          #+#    #+#             */
/*   Updated: 2023/05/29 01:29:44 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"

ircserv::ircserv(){};
ircserv::~ircserv(){};

bool	ircserv::is_num(std::string str)
{
	size_t size = str.size();
	for (size_t i = 0; i < size; i++)
	{
		if (isdigit(str[i]) == false)
			return (false);
	}
	return (true);
}

bool	ircserv::ircserv_port(char *av)
{
	std::stringstream ss(av);

	if (ss.str().size() > 5 || is_num(ss.str()) == false)
		return (false);
	ss >> port;

	if (port >= 1024 && port <= 65535)
		return (true);
	return false;
}

bool	ircserv::ircserv_start(char **av)
{
	if (ircserv_port(av[1]) == false)
		return std::cerr << "Error: invalid port" << std::endl, false;
	password = av[2];
	return true;
};