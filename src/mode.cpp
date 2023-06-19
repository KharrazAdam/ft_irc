#include "client.hpp"

# define NEG 0
# define POS 1
# define I	 4
# define T	 8
# define K	 16
# define O	 32

	// · i: Set/remove Invite-only channel
	// · t: Set/remove the restrictions of the TOPIC command to channel
	// operators
	// · k: Set/remove the channel key (password)
	// · o: Give/take channel operator privilege

void	client::flag_i(Channel& ch, bool sign)
{
	if (ch.vecFind(ch.mods, *this) == ch.mods.end())
		return send_error("ERR_CHANOPRIVSNEEDED", ch.getTitle()), (void)0; // ERR_CHANOPRIVSNEEDED // done 
	ch.set_i(sign);
}

void	client::flag_t(Channel& ch, bool sign)
{
	if (ch.vecFind(ch.mods, *this) == ch.mods.end())
		return send_error("ERR_CHANOPRIVSNEEDED", ch.getTitle()), (void)0; // ERR_CHANOPRIVSNEEDED // done 
	ch.set_t(sign);
}

void	client::flag_k(Channel& ch, bool sign, std::deque<std::string>& deq)
{
	if (ch.vecFind(ch.mods, *this) == ch.mods.end())
		return send_error("ERR_CHANOPRIVSNEEDED", ch.getTitle()), (void)0; // ERR_CHANOPRIVSNEEDED // done 
	if (sign == false)
	{
		ch.setKey("");
		ch.set_k(false);
	}
	else
	{
		if (deq.empty())
			return send_error("ERR_NEEDMOREPARAMS", "MODE"), (void)1; // ERR_NEEDMOREPARAMS // done 
		ch.setKey(deq.front());
		deq.pop_front();
	}
}

void	client::flag_o(Channel& ch, bool sign, client& cl)
{
	std::__1::vector<client *>::iterator it = ch.vecFind(ch.mods, cl);

	if (ch.vecFind(ch.users, *this) == ch.users.end())
		return send_error("ERR_USERNOTINCHANNEL" , ch.getTitle()), (void)0; // ERR_USERNOTINCHANNEL // done 
	if (ch.vecFind(ch.mods, *this) == ch.mods.end())
		return send_error("ERR_CHANOPRIVSNEEDED", ch.getTitle()), (void)0; // ERR_CHANOPRIVSNEEDED // done
	if (ch.vecFind(ch.users, cl) == ch.users.end())
		return send_error("ERR_USERNOTINCHANNEL", ch.getTitle()), (void)0; // ERR_USERNOTINCHANNEL // done
	if (sign && (it == ch.mods.end()))
		ch.mods.push_back(&cl);
	else if (!sign && (it != ch.mods.end()) && ch.mods.size() > 1)
		ch.mods.erase(it);
}

bool	client::cmd_MODE(std::deque<std::string>& deq, std::map<int, client>& cl, std::map<std::string, Channel>& ch)
{
	(void)cl;
	std::vector<std::pair<bool, char> >modes;
	bool	sign = true;
	
	if (deq.size() < 3)
		return send_error("ERR_NEEDMOREPARAMS", "MODE"), false; // ERR_NEEDMOREPARAMS // done
	deq.pop_front();
	if (ch.find(deq.front()) == ch.end())
		return send_error("ERR_NOSUCHCHANNEL", deq.front()), false; // ERR_NOSUCHCHANNEL // done
	Channel& chan = (*ch.find(deq.front())).second;
	deq.pop_front();
	for (size_t i = 0; i < deq.front().size(); i++)
	{
		if (deq.front()[i] == '-')
			sign = false;		
		else if (deq.front()[i] == '+')
			sign = true;
		else
			modes.push_back(std::pair<bool, char>(sign, deq.front()[i]));
	}
	if (modes.empty())
		return send_error("ERR_NEEDMOREPARAMS", "MODE"), false; // ERR_NEEDMOREPARAMS // done
	deq.pop_front();
	std::__1::map<int, client>::iterator it;
	for (size_t i = 0; i < modes.size(); i++)
	{
		if (modes[i].second == 'i')
			flag_i(chan, modes[i].first);
		else if (modes[i].second == 't')
			flag_t(chan, modes[i].first);
		else if (modes[i].second == 'k')
			flag_k(chan, modes[i].first, deq);
		else if (modes[i].second == 'o')
		{
			if (deq.empty())
				send_error("ERR_NEEDMOREPARAMS", "MODE"); // ERR_NEEDMOREPARAMS // Done
			else
			{
				if ((it = mapFind(cl, deq.front())) == cl.end()) // if client do not  exists
					send_error("ERR_NOSUCHNICK"); // ERR_NOSUCHNICK // Done 
				else				
					flag_o(chan, modes[i].first, (*it).second);
				deq.pop_front();
			}
		}
		else
			send_error("ERR_UNKNOWNMODE", string(1, modes[i].second)); // done
	}
	
	return true;
}
