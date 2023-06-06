/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 08:27:46 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/06 08:59:03 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# ifndef CLIENT_HPP
# define CLIENT_HPP


# include <iostream>

class client
{
private:
	bool		auth;
	std::string	username;
	std::string	nickname;
public:
	client();
	~client();
	void	SetAuth(bool);
	void	SetUser(std::string);
	void	SetNick(std::string);
	bool	GetAuth(void) const;
};

#endif