#include "client.hpp"

bool	client::cmd_USER(std::deque<std::string>& deq)
{
	(void)deq;
	if (auth & AUTHENTIFICATED)
		return send_error("ERR_ALREADYREGISTRED"), false; // ERR_ALREADYREGISTRED
	if (deq.size() < 5)
		return send_error("ERR_NEEDMOREPARAMS"), false; // ERR_NEEDMOREPARAMS
	if (deq[4][0] != ':')
		return send_error("ERROR SYNTAX"), false; // ERR_chars
	deq.pop_front();
	username = deq.front();
	for (size_t i = 0; i < 3; i++)
		deq.pop_front();
	deq.front().erase(deq.front().begin());
	while (1)
	{
		realname += deq.front();
		deq.pop_front();
		if (deq.empty() == true)
			break ;
		realname += " ";
	}
	auth |= USERNAME;
	if ((auth & NICKNAME))
	{
		auth |= AUTHENTIFICATED;
		RPL_WELCOME(); // send welcome
	}
	return true;
}