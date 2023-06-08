/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 08:27:46 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/08 17:43:24 by akharraz         ###   ########.fr       */
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
	bool	ShowAuth(void) const;
	// <-----commands------->
	bool	cmd_PASS(std::deque<std::string>&, std::string&);
	// <-----errors--------->
	void	ERR_NEEDMOREPARAMS(const char *);
	void	ERR_PASSWDMISMATCH(void);
	void	ERR_ALREADYREGISTERED(void);
};

#endif