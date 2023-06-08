/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 08:27:46 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/08 20:20:17 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <deque>
# include <sys/socket.h>
class client
{
private:
	int			fd;
	bool		auth;
	std::string	username;
	std::string	nickname;
	void	SetAuth(bool);
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
	bool	cmd_NICK(std::deque<std::string>&);
	// <-----errors--------->
	void	ERR_NEEDMOREPARAMS(const char *);
	void	ERR_REPETETIVE(const char *);
};

#endif