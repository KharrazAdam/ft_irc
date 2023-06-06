/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 16:08:52 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/06 06:01:12 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef IRCSERV_HPP
# define IRCSERV_HPP

# include <sstream>
# include <sys/socket.h>
# include <unistd.h>
# include <netinet/in.h>
# include <vector>
# include <map>
# include "client.hpp"
# include <poll.h>

# define TCP_MSS 1460
# define MAX_POLLFD 200

class ircserv
{
private:
	int						port;
	std::string				password;
	std::vector<client>		clients;
	std::map<int, client>	cl;

	bool	is_num(std::string);
	bool	ircserv_port(char *);
	bool	ircserv_pass(char *);
	bool	ircserv_run(void);
	bool	ircserv_bind(sockaddr_in6 *addr, int sock);
	bool	ircserv_receiv(int sock);
	bool	ircserv_msg(pollfd& Ps, std::string& str);
	
	//<------ a reviser--------->
	bool	ircserv_add(int client);

public:
	bool	ircserv_start(char **);
	ircserv();
	~ircserv();
};

#endif