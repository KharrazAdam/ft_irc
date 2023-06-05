/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 16:38:00 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/05 10:28:33 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"
# define TIMEOUT 1000

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

bool	ircserv::ircserv_bind(sockaddr_in6 *addr, int sock)
{
	bzero(addr, sizeof(sockaddr_in6));
	addr->sin6_family = AF_INET6;
	addr->sin6_port = htons(port); // host to network short: if it's small endian converts it to big endian	
	addr->sin6_len = sizeof(sockaddr_in6);
	if (bind(sock, (sockaddr *)addr, sizeof(sockaddr_in6)) == -1)
		return std::cerr << "Error: bind()" << std::endl, false;
	return (true);
}

bool	ircserv::ircserv_receiv(int sock)
{
	pollfd	Ps[MAX_POLLFD];
	int		rs;
	int		num;
	int		i = 0;
	int		client;

	num = 1;
	bzero(Ps, MAX_POLLFD);
	Ps[0].fd = sock;
	Ps[0].events = POLLIN;
	while (1)
	{
		i = 0;
		rs = poll(Ps, MAX_POLLFD, -1);
		if (rs == -1)
			return std::cerr << "Error: poll()" << std::endl, false;
		while (i < num)
		{
			if (Ps[i].revents & POLLIN)
			{
				if (Ps[i].fd == sock)
				{
					client = accept(sock, NULL, NULL);
					if (client == -1)
						return std::cerr << "Error: accept()" << std::endl, false;
					else
					{
						num++;
						Ps[num].fd = client;
						Ps[num].events = POLLIN;
						std::cout << "new connection established fd == {" << Ps[num].fd << "}" << std::endl;
					}
				}
			}
			i++;
		}
		
		if (true)
			;
	}
	(void)sock;
	return (true);
}

bool	ircserv::ircserv_run(void)
{
	int				sock;
	sockaddr_in6	addr;

	sock = socket(PF_INET6, SOCK_STREAM, 0);
	if (sock == -1)
		return std::cerr << "Error: socket()" << std::endl, false;
	if (ircserv_bind(&addr, sock) == false)
		return close(sock), false;
	if (listen(sock, SOMAXCONN) == -1)
		return std::cerr << "Error: listen()" << std::endl, false;	
	if (ircserv_receiv(sock) == false)
		return false;
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
