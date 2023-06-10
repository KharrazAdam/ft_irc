/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 16:08:52 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/08 20:27:33 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef IRCSERV_HPP
# define IRCSERV_HPP

# include <sstream>
# include <sstream>
# include <sys/socket.h>
# include <unistd.h>
# include <netinet/in.h>
# include <vector>
# include <map>
# include <deque>
# include "client.hpp"
# include <poll.h>

# define TCP_MSS 1460
# define MAX_POLLFD 200

class ircserv
{
private:
	int						port;
	std::string				password;
	std::map<int, client>	cl;

	bool	is_num(std::string);
	bool	ircserv_port(const char*, const char *);
	bool	ircserv_run(void);
	bool	ircserv_bind(sockaddr_in6 *addr, int sock);
	bool	ircserv_serv(int sock);
	bool	ircserv_receiv(pollfd& Ps);
	bool	ircserv_connect(pollfd& Ps, int sock, int *num);
	bool	ircserv_msg(pollfd& Ps, std::string& str);
	bool	ircserv_cmd(std::deque<std::string>&, std::string str);
	
	//<------ a reviser--------->

public:
	bool	ircserv_start(char **);
	ircserv();
	~ircserv();
};

#endif