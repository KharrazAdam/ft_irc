#include "client.hpp"

/**
 * @note please recode this function in a clean way
*/
bool	Client::cmd_NICK(std::deque<std::string>& deq, std::map<int, Client>& cl)
{
	std::map<int, Client>::iterator it = cl.begin();

	if (deq.size() == 1)
		return send_error("ERR_NONICKNAMEGIVEN"), false; // ERR_NONICKNAMEGIVEN // done
	deq.pop_front();
	if (deq.front().front() == ':')
		deq.front().erase(0, 1);
	if (deq.size() > 1 || deq.front().size() < 3 || deq.front()[0] == '#' || deq.front()[0] == ':' || deq.front().substr(0, 2) == "#&" || deq.front().substr(0, 2) == "&#")
		return send_error("ERR_ERRONEUSNICKNAME"), false; // ERR_ERRONEUSNICKNAME
	while (it != cl.end())
	{
		if ((*it).second.nickname == deq.front())
			return send_message(::string(":startimes42 433 * "+deq.front()+" is already in use.\r\n")), false; // ERR_NICKNAMEINUSE	
		it++;
	}
	if (auth & NICKNAME)
		return send_message(":" + nickname + "!"+username+"@"+getAddr()+" NICK " + deq.front() + "\r\n"), nickname = deq.front(), true;
	nickname = deq.front();
	if ((auth & USERNAME))
	{
		auth |= AUTHENTIFICATED;
		RPL_WELCOME(); // send welcome
	}
	auth |= NICKNAME;
	return true;
}