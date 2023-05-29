/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 16:09:41 by akharraz          #+#    #+#             */
/*   Updated: 2023/05/29 01:25:14 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"

# include <sys/socket.h>
# include <unistd.h>

int	main(int ac, char **av)
{
	ircserv irc;

	if (ac != 3)
		return std::cerr << "Error: \"<port>\" \"<password>\"" << std::endl, 1;
	if (irc.ircserv_start(av) == false)
		return (1);
	// std::cout << irc.port << std::endl;
	// sockaddr*	name;
	// int s_fd = socket(PF_INET6, SOCK_STREAM, 0);
	// if (s_fd == -1)
	// 	return std::cerr << "Error: socket() failed" << std::endl, 1;
	// // bind(s_fd,)
	// close(s_fd);
	return (0);
}
