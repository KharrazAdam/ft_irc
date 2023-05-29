/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 16:09:41 by akharraz          #+#    #+#             */
/*   Updated: 2023/05/29 01:18:09 by akharraz         ###   ########.fr       */
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
	close(s_fd);
	return (0);
}
