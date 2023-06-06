/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 16:38:00 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/06 04:34:20 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"
# define TIMEOUT 1000

ircserv::ircserv(){};
ircserv::~ircserv(){};

/**
 * @brief checks if all the str's elemnts are digits
 * @return true if str is number otherwise false 
*/
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

/**
 * @param av is the second command line's arg
 * @brief checks if the param is number and checks for it's range 1024 -> 65535
 * @return true if valid or false otherwise
*/
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

/**
 * @param addr the struct used to store socket's info
 * @param sock is the file descriptor of server's socket
 * @brief binds the socket to the provided port and the host's IP adress
 * @return false if binding faild. otherwise true
*/
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

/**
 * @param pollfd structur that holds client fd and events that can be monitored on the fd
 * @param i index
 * @brief store message in str to be used later
 * @return false if syscall recv() faild
*/
bool	ircserv::ircserv_msg(pollfd* Ps, int i)
{
	std::string	str;
	int			rs;
	char		buffer[1024];
	
	str.clear();
	rs = recv(Ps[i].fd, buffer, 1024, 0);
	if (rs == -1)
		return std::cerr << "Error: recv()" << std::endl, false;
	if (rs == 0)
	{
		std::cerr << "client disconnnected" << std::endl;
		close(Ps[i].fd);
		Ps[i].fd = -1;
	}
	str.append(buffer, rs);
	return (true);
}

/**
 * @param socket the srver's socket fd
 * @brief accepts new connctions and receive messages from clients
 * @return false if syscall fail otherwise the loop keeps looping
*/
bool	ircserv::ircserv_receiv(int sock)
{
	pollfd	Ps[MAX_POLLFD];
	int		num;
	int		i = 0;
	int		client;
	int		rs;

	num = 1;
	bzero(Ps, MAX_POLLFD);
	Ps[0].fd = sock;
	Ps[0].events = POLLIN;
	while (1)
	{
		rs = poll(Ps, MAX_POLLFD, -1);
		if (rs == -1)
			return std::cerr << "Error: poll()" << std::endl, false;
		i = 0;
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
						Ps[num].fd = client;
						Ps[num].events = POLLIN;
						std::cout << "new connection established fd == {" << Ps[num].fd << "}" << std::endl;
						send(Ps[num].fd, "marhba bik m3ana fserver hhhhhh yalah dakhal password albatal: ", 63, 0);
						num++;
					}
				}
				else
				{
					if (ircserv_msg(Ps, i) == false)
						return false;
				}
			}
			i++;
		}
	}
	return (true);
}

/**
 * @brief binds the socket, lListens, and receiving connections
 * @return false if any inconvenience occurs 
*/
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
		return std::cerr << "Error: listen()" << std::endl, close(sock), false;	
	if (ircserv_receiv(sock) == false)
		return close(sock), false;
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
