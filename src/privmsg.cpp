#include "client.hpp"

bool Client::print_pvmsg(int fd, std::string& message, std::string& receiver, bool isChannel)
{
	string msg;
	if (isChannel)
		msg = ":" + nickname + "!"+nickname+"@" + getAddr() + " PRIVMSG " + receiver + " :" + message + "\r\n";
	else
		msg = ":" + nickname + "!"+nickname+"@" + getAddr() + " PRIVMSG " + receiver + " :" + message + "\r\n";
	if (send(fd, msg.c_str(), msg.size(), 0) == -1)
		return false;
	return true;
}


bool	Client::msgCl(std::map<int, Client>& cl, std::string& message, std::string& receiver)
{
	std::map<int, Client>::iterator it;

	for (it = cl.begin(); it != cl.end(); it++)
	{
		if (receiver == (*it).second.getNick())
			break ;
	}
	if (it == cl.end())
		return send_error("ERR_NOSUCHNICK"), false; // ERR_NOSUCHNICK // done
	else if (!print_pvmsg((*it).second.getFd(), message, receiver, false))
		return send_message("ERR_SYSCALL_SEND"), false; // ERR_SYSCALL_SEND // done
	return true;
}

bool	Client::msgCh(std::map<std::string, Channel>& ch, std::string& message, std::string& receiver)
{
	if (ch.find(receiver) == ch.end())
		return send_error("ERR_NOSUCHCHANNEL" , receiver), false; // ERR_NOSUCHCHANNEL // done
	else
	{
		if (ch[receiver].vecFind(ch[receiver].users, *this) == ch[receiver].users.end()) /// not a cmember
			return send_error("ERR_CANNOTSENDTOCHAN" , receiver), false; // ERR_CANNOTSENDTOCHAN // done
		for (size_t j = 0; j < ch[receiver].users.size(); j++)
		{
			if ((*this).getNick() == ch[receiver].users[j]->getNick())
				continue ;
			if (!print_pvmsg(ch[receiver].users[j]->getFd(), message, receiver, true))
				send_error("ERR_NOSUCHCHANNEL" , receiver); // DONE
		}
	}
	return true;
}

bool	Client::cmd_PRIVMSG(std::deque<std::string>& deq, std::map<int, Client>& cl, std::map<std::string, Channel>& ch)
{
	std::vector<std::string>	receivers;
	std::string	message("");


	if (deq.size() < 3)
		return send_error("ERR_NOTEXTTOSEND"), false; // ERR_NOTEXTTOSEND 		// Done
	com_sep(deq, receivers);
	deq.pop_front();
	while (true)
	{
		message.append(deq.front());
		message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());
		deq.pop_front();
		if (deq.empty() == true)
			break ;
		message.append(" ");
	}
	if (message.front() == ':')
		message.erase(0, 1);
	for (size_t i = 0; i < receivers.size(); i++)
	{
		if (receivers[i][0] == '#')
			msgCh(ch, message, receivers[i]);
		else
			msgCl(cl, message, receivers[i]);
	}
	return true;
}
