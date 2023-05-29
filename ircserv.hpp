/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 16:08:52 by akharraz          #+#    #+#             */
/*   Updated: 2023/05/29 01:24:52 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef IRCSERV_HPP
# define IRCSERV_HPP

# include <iostream>
# include <sstream>

class ircserv
{
private:
	int	port;
	std::string	password;

	bool	is_num(std::string);
	bool	ircserv_port(char *);
	bool	ircserv_pass(char *);
public:
	ircserv();
	~ircserv();
	bool	ircserv_start(char **);
};

#endif