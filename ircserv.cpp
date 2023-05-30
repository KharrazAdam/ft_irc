/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 16:38:00 by akharraz          #+#    #+#             */
/*   Updated: 2023/05/30 16:55:40 by akharraz         ###   ########.fr       */
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
		return (std::cerr << "Error: invalid port" << std::endl, false);
	ss >> port;

	if (port >= 1024 && port <= 65535)
		return (true);
	return std::cerr << "Error: port out of range: 1024 -> 65535" << std::endl, false;
}

bool ircserv::ircserv_bind(sockaddr_in6 *addr, int sock)
{
	bzero(addr, sizeof(sockaddr_in6));
	addr->sin6_family = AF_INET6;
	addr->sin6_port = htons(port); // host to network short: if it's small endian converts it to big endian	
	addr->sin6_len = sizeof(sockaddr_in6);
	if (bind(sock, (sockaddr *)addr, sizeof(sockaddr_in6)) == -1)
		return std::cerr << "Error: bind()" << std::endl, false;
	return (true);
}

bool	ircserv::ircserv_run(void)
{
	int	sock;
	sockaddr_in6 addr;

	sock = socket(PF_INET6, SOCK_STREAM, 0);
	if (sock == -1)
		return std::cerr << "Error: socket()" << std::endl, false;

	if (ircserv_bind(&addr, sock) == false)
		return close(sock), false;

	if (listen(sock, SOMAXCONN) == -1)
		return std::cerr << "Error: listen()" << std::endl, false;	

	while (accept(sock, NULL, NULL) != -1)
	{
		printf("salam\n");
	}
	return (close(sock), true);
}

bool	ircserv::ircserv_start(char **av)
{
	if (ircserv_port(av[1]) == false)
		return false;
	password = av[2];
	if (ircserv_port(av[1]) == false)
		return false;
	if (ircserv_run() == false)
		return false;
	return true;
};
