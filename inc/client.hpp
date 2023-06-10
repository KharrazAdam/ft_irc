/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 08:27:46 by akharraz          #+#    #+#             */
<<<<<<< HEAD
<<<<<<< HEAD
/*   Updated: 2023/06/09 20:11:11 by akharraz         ###   ########.fr       */
=======
/*   Updated: 2023/06/08 20:20:17 by akharraz         ###   ########.fr       */
>>>>>>> b9b2f1660b9844703759541c9af40f437c31ac1c
=======
/*   Updated: 2023/06/08 20:20:17 by akharraz         ###   ########.fr       */
>>>>>>> b9b2f1660b9844703759541c9af40f437c31ac1c
/*                                                                            */
/* ************************************************************************** */


# ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <deque>
# include <map>
# include <sys/socket.h>
class client
{
private:
	// <nick!user@host>
	int			fd;
	bool		auth;
	// bool		registred;
	std::string	username;
	std::string	nickname;
	std::string	addr;

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
<<<<<<< HEAD
<<<<<<< HEAD
	bool	cmd_NICK(std::deque<std::string>&, std::map<int, client>& cl);
	bool	cmd_USER(std::deque<std::string>&);
	// <-----errors--------->
	
	void	send_error(const char *) const;
	void	send_message(const char* er) const;
=======
=======
>>>>>>> b9b2f1660b9844703759541c9af40f437c31ac1c
	bool	cmd_NICK(std::deque<std::string>&);
	// <-----errors--------->
	void	ERR_NEEDMOREPARAMS(const char *);
	void	ERR_REPETETIVE(const char *);
<<<<<<< HEAD
>>>>>>> b9b2f1660b9844703759541c9af40f437c31ac1c
=======
>>>>>>> b9b2f1660b9844703759541c9af40f437c31ac1c
};

#endif