#include "client.hpp"

// 
//   :dan!~h@localhost PRIVMSG #coolpeople :Hi everyone!
//                                   ; Message from dan to the channel
//                                   #coolpeopl
bool client::print_pvmsg(int fd, std::string& message, std::string& receiver, bool isChannel)
{
	string msg;
	if (isChannel)
		msg = ":" + nickname + "!"+nickname+"@" + "localhost" + " PRIVMSG " + receiver + " :" + message + "\r\n";
	else
		msg = ":" + nickname + " PRIVMSG " + receiver + " :" + message + "\r\n";
	if (send(fd, msg.c_str(), msg.size(), 0) == -1)
		return false;
	return true;
}


bool	client::msgCl(std::map<int, client>& cl, std::string& message, std::string& receiver)
{
	std::map<int, client>::iterator it;

	for (it = cl.begin(); it != cl.end(); it++)
	{
		if (receiver == (*it).second.getNick())
			break ;
	}
	if (it == cl.end())
		return send_error("ERR_NOSUCHNICK"), false;
	else if (!print_pvmsg((*it).second.getFd(), message, receiver, false))
		return send_error("ERR_SYSCALL_SEND"), false;
	return true;
}

bool	client::msgCh(std::map<std::string, Channel>& ch, std::string& message, std::string& receiver)
{
	if (ch.find(receiver) == ch.end())
		return send_error("ERR_NOSUCHCHANNEL"), false;
	else
	{
		if (ch[receiver].vecFind(ch[receiver].users, *this) == ch[receiver].users.end()) /// not a cmember
			return send_error("ERR_CANNOTSENDTOCHAN"), false;
		for (size_t j = 0; j < ch[receiver].users.size(); j++)
		{
			
			if (!print_pvmsg(ch[receiver].users[j]->getFd(), message, receiver, true))
				send_error("ERR_SYSCALL_SEND");
		}
	}
	return true;
}

bool	client::cmd_PRIVMSG(std::deque<std::string>& deq, std::map<int, client>& cl, std::map<std::string, Channel>& ch)
{
	std::vector<std::string>	receivers;
	std::string	message("");


	if (deq.size() < 3)
		return send_error(":irc.example.com 433 * ben :Nickname is already in use"), false; // ERR_NOTEXTTOSEND
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
			msgCh(ch, message, receivers[i]);
		else
			msgCl(cl, message, receivers[i]);
	}
	return true;
}
