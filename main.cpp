/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 16:09:41 by akharraz          #+#    #+#             */
/*   Updated: 2023/05/29 21:48:42 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"

int	main(int ac, char **av)
{
	ircserv irc;

	if (ac != 3)
		return std::cerr << "Error: \"./ircserv <port> <password>\"" << std::endl, 1;
	if (irc.ircserv_start(av) == false)
		return (1);
	return (0);
}
