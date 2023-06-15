/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 17:53:42 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/15 08:26:39 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"
#include "channel.hpp"


bool	client::cmd_SHOW(std::deque<std::string>& deq, std::map<std::string, Channel>& chan)
{
	deq.pop_front();
	
	if (chan.find(deq.front()) != chan.end())
		chan[deq.front()].show_details();
	return true;
}
