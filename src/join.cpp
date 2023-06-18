/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 17:53:42 by akharraz          #+#    #+#             */
/*   Updated: 2023/06/18 04:32:58 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"
#include "channel.hpp"

bool	Channel::addMod(client& cl)
{
	mods.push_back(&cl);
	return true;
}


bool	client::cmd_JOIN(std::deque<std::string>& deq, std::map<std::string, Channel>& channels)
{
	std::vector<std::string>	titles;
	std::vector<std::string>	keys;

	if (deq.size() == 1)
		return send_error("ERR_NEEDMOREPARAMS"), false; // ERR_NEEDMOREPARAMS
	com_sep(deq, titles);
	com_sep(deq, keys);
	for (size_t i = keys.size(); i < titles.size(); i++)
		keys.push_back("");
	for (size_t i = 0; i < titles.size(); i++)
	{
		if (channels.find(titles[i]) == channels.end())
		{
			channels[titles[i]] = ::Channel(titles[i], keys[i]); // channel doesn't exist
			channels[titles[i]].addMod(*this);
		}
		if (!channels[titles[i]].GetKey().compare("") || !channels[titles[i]].GetKey().compare(keys[i]))
			channels[titles[i]].add_user(*this);
		else	
			send_error("ERR_BADCHANNELKEY");
	}
	return true;
}
