/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 08:27:46 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/07 18:26:21 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <deque>

class client
{
private:
	bool		auth;
	std::string	username;
	std::string	nickname;
	void	SetAuth(bool);
	void	SetUser(std::string);
	void	SetNick(std::string);
public:
	client();
	~client();
	bool	ShowAuth(void) const;
	void	cmd_pass(std::deque<std::string>&);
};

#endif