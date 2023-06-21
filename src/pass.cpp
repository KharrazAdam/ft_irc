#include "client.hpp"

bool	Client::cmd_PASS(std::deque<std::string>& deq, std::string& pass)
{
	if (auth & PASSWORD)
		return send_error("ERR_ALREADYREGISTERED"), true; // ERR_ALREADYREGISTERED // done
	if (deq.size() == 1)
		return send_error("ERR_NEEDMOREPARAMS", "PASS"), false; // ERR_NEEDMOREPARAMS // done
	deq.pop_front();
	if (deq.front()[0] == ':')
		deq.front().erase(deq.front().begin());
	if (deq.size() > 1 || deq.front().compare(pass) != 0)
		return send_error("ERR_PASSWDMISMATCH"), false; // ERR_PASSWDMISMATCH // done
	return auth |= PASSWORD, true;
}