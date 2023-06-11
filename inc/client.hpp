/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 08:27:46 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/11 01:47:46 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <deque>
# include <map>
# include <sys/socket.h>

# define PASSWORD 1
# define USERNAME 2
# define NICKNAME 4
# define AUTHENTIFICATED 8
# define REGISTRED 16
class client
{
private:
	// <nick!user@host>
	int			fd;
	short		auth;
	std::string	username;
	std::string	nickname;
	std::string	realname;
	std::string	addr;

	void	SetAuth(int);
	void	SetUser(std::string);
	void	SetNick(std::string);
public:
	client(int);
	client();
	~client();
	bool			ShowAuth(void) const;
	std::string&	getNick(void);
	// <-----commands------->
	
	bool	cmd_PASS(std::deque<std::string>&, std::string&);
	bool	cmd_NICK(std::deque<std::string>&, std::map<int, client>& cl);
	bool	cmd_USER(std::deque<std::string>&);
	// <-----errors--------->
	
	void	send_error(const char *) const;
	void	send_message(const char* er) const;
	void	RPL_WELCOME(void);
};

#endif