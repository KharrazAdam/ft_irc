/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 16:08:52 by akharraz          #+#    #+#             */
/*   Updated: 2023/05/30 20:21:16 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef IRCSERV_HPP
# define IRCSERV_HPP

# include <iostream>
# include <sstream>
# include <sys/socket.h>
# include <unistd.h>
# include <netinet/in.h>

#define TCP_MSS 1460
class ircserv
{
private:
	int	port;
	std::string	password;

	bool	is_num(std::string);
	bool	ircserv_port(char *);
	bool	ircserv_pass(char *);
	bool	ircserv_run(void);
	bool	ircserv_bind(sockaddr_in6 *addr, int sock);
public:
	ircserv();
	~ircserv();
	bool	ircserv_start(char **);
};

#endif