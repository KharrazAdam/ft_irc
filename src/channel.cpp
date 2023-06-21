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

void Channel::set_l(int l)
{
	this->l = l;
}

std::vector<Client *>::iterator	Channel::vecFind(std::vector<Client *>& vec, Client& cl)
{
	std::vector<Client *>::iterator it;

    for (it = vec.begin(); it != vec.end(); it++)
	{
		if ((*it)->getNick() == cl.getNick())
			break ;
	}
	return it;
}

std::vector<Client *>::iterator	Channel::vecFind(std::vector<Client *>& vec, std::string &nick)
{
	std::vector<Client *>::iterator it;

    for (it = vec.begin(); it != vec.end(); it++)
	{
		if ((*it)->getNick() == nick)
			break ;
	}
	return it;
}

string get_mods(vector<Client *> &mods)
{
    string msg = "@";
    for (size_t i = 0; i < mods.size(); i++)
    {
        msg += mods[i]->getNick();
        if (i != mods.size() - 1)
            msg += " @";
    }
    return msg;
}

string Channel::join_msg(string nickname,string username, string adrs, string moders)
{
	string msg = ":" + nickname + "!" + username + "@"+adrs+" JOIN " + this->title + "\r\n";
	msg += ":startimes42 MODE o +o\r\n";
	msg += ":startimes42 353 " + nickname + " = " + this->title + " :"+ moders +"\r\n";
	msg += ":startimes42 366 " + nickname + " " + this->title + " :End of /NAMES list.\r\n";
	return msg;
}

string Channel::join_msg_exi(string nickname,string username, string adrs, string moders)
{
	string msg = ":" + nickname + "!" + username + "@"+adrs+" JOIN " + this->title + "\r\n";
	msg += ":startimes42 332 " + nickname + " " + this->title + " :" + (this->topic.empty() ? "No topic is set" : this->topic) + "\r\n";
	msg += ":startimes42 333 " + nickname + " " + this->title + " " + nickname + "!"+ username+"@"+adrs+"\r\n";
	
	std::vector<Client *>::iterator it;
	msg += ":startimes42 353 " + nickname + " = " + this->title + " :@";
	for (it = mods.begin(); it != mods.end(); it++)
	{
		msg += (*it)->getNick();
		if (it != mods.end() - 1)
			msg += " @";
	}
	msg += " ";
	for (it = users.begin(); it != users.end(); it++)
	{
		if ((this->vecFind(mods, (*it)->getNick()) == mods.end()))
		{
			msg += (*it)->getNick();
			if (it != users.end() - 1)
				msg += " ";
		}
	}
	msg += "\r\n";
	msg += ":startimes42 366 " + nickname + " " + this->title + " :End of /NAMES list.\r\n";
	(void)moders;
	//inform members
	return msg;
}

void Channel::send_msg(string msg)
{
	for (size_t i = 0; i < users.size(); i++)
	{
		users[i]->send_message(msg);
	}
}

bool	Channel::add_user(Client &cl)
{   
	std::vector<Client *>::iterator	it;

	if (vecFind(users, cl) != users.end())
		return true;
	if (l > 0 && users.size() == (size_t)this->l)
			return (cl.send_error("ERR_CHANNELISFULL", title), false); // done !
	if (this->i == true)
	{
		if ((it = vecFind(invited, cl)) == invited.end())
			return (cl.send_error("ERR_INVITEONLYCHAN", title), false); // done !
		invited.erase(it);
	}
	users.push_back(&cl);
	if (users.size() == 1) // new channel
	{
		cl.send_message(join_msg(cl.getNick(),cl.getUsername(), cl.getAddr() ,get_mods(this->mods)));
		//send_msg(":" + cl.getNick() + "!" + cl.getUsername() + "@localhost JOIN " + this->title + "\r\n");
	}
	else // existed channel
	{
		cl.send_message(join_msg_exi(cl.getNick(),cl.getUsername(),cl.getAddr(), get_mods(this->mods)));
		send_msg(":" + cl.getNick() + "!" + cl.getUsername() + "@"+cl.getAddr()+" JOIN " + this->title + "\r\n");
	}
	if (mods.size() == 0)
		mods.push_back(&cl);
	return true;
}

bool Channel::kickUser(std::string &nick, Client &cl, std::string &reason)
{
	Client *tmp;
	if (vecFind(users, cl) == users.end())
	{
		cl.send_error("ERR_NOTONCHANNEL", title); // done !
		return false;
	}
	if (vecFind(users, nick) == users.end())
	{
		cl.send_error("ERR_USERNOTINCHANNEL", title); // done !
		return false;
	}
	tmp = *vecFind(users, nick);
	users.erase(vecFind(users, nick));
	tmp->send_message(":" + cl.getNick() + "!" + cl.getUsername() + "@"+cl.getAddr()+" KICK " + this->title + " " + nick + " :" + reason + "\r\n");
	return true;
}

bool Channel::setTopic(std::string &topic, Client &cl)
{
	if (vecFind(users, cl) == users.end())
	{
		cl.send_error("ERR_NOTONCHANNEL", title); // done !
		return false;
	}
    if(t && vecFind(mods, cl) == mods.end())
	{
		cl.send_error("ERR_CHANOPRIVSNEEDED", title); // done !
		return false;
	}
	this->topic = topic;
	return true;
}

bool Channel::isMod(Client &cl)
{
	return vecFind(mods, cl) != mods.end();
}

bool Channel::isUser(Client &cl)
{
	return vecFind(users, cl) != users.end();
}

bool Channel::isInvited(Client &cl)
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

bool Channel::inviteUser(Client &cl)
{
	if (isInvited(cl))
		return true;
	invited.push_back(&cl);
	return true;
}
