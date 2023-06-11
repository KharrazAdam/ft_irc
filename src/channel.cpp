/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 17:11:18 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/11 23:23:12 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

Channel::Channel()
{
	
}

Channel::~Channel()
{
	
}

bool	Channel::add_user(client& cl)
{
	members[cl.getFd()] = cl.getNick();
	return true;
}
