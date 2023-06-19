#include "channel.hpp"

// this verstion works with limechat and adium

Channel::Channel():title(""), key(""), topic(""), l(-1), i(false), t(false){};
Channel::Channel(std::string T, std::string K):title(T), key(K), topic(""), l(-1), k(false), i(false), t(false)
{
	if (key.compare("") != 0)
		k = true;
}

Channel::~Channel()
{}

std::string&	Channel::GetKey(void)
{
	return key;
}

std::string&	Channel::getTitle(void)
{
	return title;
}

void	Channel::set_i(bool x)
{
	this->i = x;
}

void	Channel::set_t(bool x)
{
	this->t = x;
}

void	Channel::set_k(bool x)
{
	this->k = x;
}

void	Channel::setKey(std::string str)
{
	key = str;
}

std::vector<client *>::iterator	Channel::vecFind(std::vector<client *>& vec, client& cl)
{
	std::vector<client *>::iterator it;

    for (it = vec.begin(); it != vec.end(); it++)
	{
		if ((*it)->getNick() == cl.getNick())
			break ;
	}
	return it;
}

std::vector<client *>::iterator	Channel::vecFind(std::vector<client *>& vec, std::string &nick)
{
	std::vector<client *>::iterator it;

    for (it = vec.begin(); it != vec.end(); it++)
	{
		if ((*it)->getNick() == nick)
			break ;
	}
	return it;
}
string Channel::join_msg(string nickname,string username)
{
	string msg;
	msg = ":" + nickname + "!" + username + "@localhost JOIN " + this->title + "\r\n";
	msg += ":irc.localhost 353 " + nickname + " = " + this->title + " \r\n";
	msg += ":irc.localhost 366 " + nickname + " " + this->title + " :End of /NAMES list.\r\n";
	return msg;
}

string Channel::join_msg_exi(string nickname,string username)
{
	string msg;
	msg = ":" + nickname + "!" + username + "@localhost JOIN " + this->title + "\r\n";
	msg += ":irc.example.com 332 " + nickname + " " + this->title + " :" + "this->topic bbbbbbbbbbb" + "\r\n";
	msg += ":irc.example.com 333 " + nickname + " " + this->title + " " + nickname + " 1610732239\r\n";
	msg += ":irc.example.com 353 " + nickname + " = " + this->title + " :" + nickname + "\r\n";
	msg += ":irc.example.com 366 " + nickname + " " + this->title + " :End of /NAMES list.\r\n";
	return msg;
}

void Channel::send_msg(string msg)
{
	for (size_t i = 0; i < users.size(); i++)
	{
		users[i]->send_message(msg);
	}
}

bool	Channel::add_user(client &cl)
{   
	std::vector<client *>::iterator	it;

	if (vecFind(users, cl) != users.end())
		return true;
	if (l > 0 && users.size() == (size_t)this->l)
			return (cl.send_error("ERR_CHANNELISFULL"), false);
	if (this->i == true)
	{
		if ((it = vecFind(invited, cl)) == invited.end())
			return (cl.send_error("ERR_INVITEONLYCHAN"), false);
		invited.erase(it);
	}
	users.push_back(&cl);
	if (users.size() == 1) // new channel
	{
		cl.send_message(join_msg(cl.getNick(),cl.getNick()));
		send_msg(":" + cl.getNick() + "!" + cl.getUsername() + "@localhost JOIN " + this->title + "\r\n");
	}
	else // existed channel
	{
		cl.send_message(join_msg(cl.getNick(),cl.getNick()));
		send_msg(":" + cl.getNick() + "!" + cl.getUsername() + "@localhost JOIN " + this->title + "\r\n");
	}
	if (mods.size() == 0)
		mods.push_back(&cl);
	return true;
}

bool Channel::kickUser(std::string &nick, client &cl)
{
	if (vecFind(users, cl) == users.end())
	{
		cl.send_error("ERR_NOTONCHANNEL");
		return false;
	}
	if (vecFind(users, nick) == users.end())
	{
		cl.send_error("ERR_USERNOTINCHANNEL");
		return false;
	}
	users.erase(vecFind(users, nick));
	return true;
}

bool Channel::setTopic(std::string &topic, client &cl)
{
	if (vecFind(users, cl) == users.end())
	{
		cl.send_error("ERR_NOTONCHANNEL");
		return false;
	}
    if(t && vecFind(mods, cl) == mods.end())
	{
		cl.send_error("ERR_CHANOPRIVSNEEDED");
		return false;
	}
	this->topic = topic;
	return true;
}

bool Channel::isMod(client &cl)
{
	return vecFind(mods, cl) != mods.end();
}

bool Channel::isUser(client &cl)
{
	return vecFind(users, cl) != users.end();
}

bool Channel::isInvited(client &cl)
{
	return vecFind(invited, cl) != invited.end();
}

bool Channel::isActive(const char &mode)
{
	if (mode == 'i')
		return i;
    if (mode == 't')
		return t;
	if (mode == 'l')
		return l != -1;
	return true;
}

bool Channel::inviteUser(client &cl)
{
	if (isInvited(cl))
		return true;
	invited.push_back(&cl);
	return true;
}