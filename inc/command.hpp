/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 14:32:09 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/07 09:40:58 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "client.hpp"

class command
{
private:
	/* data */
public:
	command(/* args */);
	~command();
	bool	com_pass(std::string&);
};
#endif