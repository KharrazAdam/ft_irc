#include "client.hpp"

bool Client::print_notice(int fd, std::string& message, std::string& receiver, bool isChannel)
{
	string msg;
	if (isChannel)
		msg = ":" + nickname + "!"+nickname+"@" + getAddr() + " NOTICE " + receiver + " :" + message + "\r\n";
	else
		msg = ":" + nickname + " NOTICE " + receiver + " :" + message + "\r\n";
	if (send(fd, msg.c_str(), msg.size(), 0) == -1)
		return false;
	return true;
}


bool	Client::notice_msg_cl(std::map<int, Client>& cl, std::string& message, std::string& receiver)
{
	std::map<int, Client>::iterator it;

	for (it = cl.begin(); it != cl.end(); it++)
	{
		if (receiver == (*it).second.getNick())
			break ;
	}
	if (it == cl.end())
		return false;
	else if (!print_notice((*it).second.getFd(), message, receiver, false))
		return false;
	return true;
}

bool	Client::notice_msg_ch(std::map<std::string, Channel>& ch, std::string& message, std::string& receiver)
{
	if (ch.find(receiver) == ch.end())
		return false; // ERR_NOSUCHCHANNEL // done
	else
	{
		if (ch[receiver].vecFind(ch[receiver].users, *this) == ch[receiver].users.end()) /// not a cmember
			return false;
		for (size_t j = 0; j < ch[receiver].users.size(); j++)
		{
			if ((*this).getNick() == ch[receiver].users[j]->getNick())
				continue ;
			if (!print_notice(ch[receiver].users[j]->getFd(), message, receiver, true))
				return false;
		}
	}
	return true;
}

bool	Client::cmd_NOTICE(std::deque<std::string>& deq, std::map<int, Client>& cl, std::map<std::string, Channel>& ch)
{
	std::vector<std::string>	receivers;
	std::string	message("");

	if (deq.size() < 3)
		return false;
	com_sep(deq, receivers);
	deq.pop_front();
	while (true)
	{
		message.append(deq.front());
		deq.pop_front();
		if (deq.empty() == true)
		{	
			message.append("\n");
			break ;
		}
		message.append(" ");
	}
	for (size_t i = 0; i < receivers.size(); i++)
	{
		if (receivers[i][0] == '#')
			notice_msg_ch(ch, message, receivers[i]);
		else
			notice_msg_cl(cl, message, receivers[i]);
	}
	return true;
}
