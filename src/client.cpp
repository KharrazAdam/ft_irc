#include "client.hpp"

bool	is_alpha_valid(std::string& str)
{
	int	size = str.size();

	for (int i = 0; i < size; i++)
		if (std::isalpha(str[i]) == false && str[i] != ' ')
			return false;
	return true;
}

std::map<int, client>::iterator	client::mapFind(std::map<int, client>&map, std::string& nick)
{
	std::map<int, client>::iterator it;

    for (it = map.begin(); it != map.end(); it++)
	{
		if ((*it).second.getNick() == nick)
			break ;
	}
	return it;
}

/**
 * @name Deafault constructor
 * @brief set auth to false; auth will be changed to 'true' when client will insert correct password
*/
client::client(int fds):fd(fds), auth(0), username(""), nickname(""), addr("nickname!user@host"){}
client::client():auth(0), username(""), nickname(""), addr("nickname!user@host"){}

client::~client(){}

/**
 * @brief change auth to 'status';
*/
void	client::SetAuth(int status)
{
	this->auth |= status;
}

/**
 * @brief set nickname with the given string
*/
void	client::SetNick(std::string str)
{
	this->nickname = str;
}

/**
 * @brief set username with the given string
*/
void	client::SetUser(std::string str)
{
	this->username = str;
}

/**
 * @brief return the auth (either true or false)
*/
int	client::ShowAuth(void)
{
	return auth;
}

std::string&	client::getNick(void)
{
	return nickname;
}

std::string&	client::getUsername(void)
{
	return username;
}

int	client::getFd(void)
{
	return fd;
}

// COMMANDS
/**
 * @note please recode this function in a clean way
*/
bool	client::cmd_PASS(std::deque<std::string>& deq, std::string& pass)
{
	if (auth & PASSWORD)
		return send_error("ERR_ALREADYREGISTERED"), true; // ERR_ALREADYREGISTERED  // done !
	if (deq.size() == 1)
		return send_error("ERR_NEEDMOREPARAMS", "PASS"), false; // ERR_NEEDMOREPARAMS  // done !
	deq.pop_front();
	// if (deq.size() > 1 || deq.front().compare(pass) != 0)
	// 	return send_error("ERR_PASSWDMISMATCH"), false; // ERR_PASSWDMISMATCH // done !
	(void)pass;
	return auth |= PASSWORD, true;
}

/**
 * @note please recode this function in a clean way
*/
bool	client::cmd_NICK(std::deque<std::string>& deq, std::map<int, client>& cl)
{
	std::map<int, client>::iterator it = cl.begin();

	if (deq.size() == 1)
		return send_error("ERR_NONICKNAMEGIVEN"), false; // ERR_NONICKNAMEGIVEN  // done !
	deq.pop_front();
	if (deq.size() > 1 || deq.front().size() < 3 || deq.front()[0] == '#' || deq.front()[0] == ':' || deq.front().substr(0, 2) == "#&" || deq.front().substr(0, 2) == "&#")
		return send_error("ERR_ERRONEUSNICKNAME"), false; // ERR_ERRONEUSNICKNAME  // done !
	while (it != cl.end())
	{
		if ((*it).second.nickname == deq.front())
			return send_message(::string(":startimes42 433 * "+deq.front()+" is already in use.\r\n")), false; // ERR_NICKNAMEINUSE	
		it++;
	}
	nickname = deq.front();
	auth |= NICKNAME;
	if ((auth & USERNAME))
	{
		auth |= AUTHENTIFICATED;
		RPL_WELCOME(); // send welcome
	}
	//return send_message("NICK NAME DONE"), true;
	return true;
}

bool	client::cmd_USER(std::deque<std::string>& deq)
{
	(void)deq;
	// if (auth & AUTHENTIFICATED)
	// 	return send_error("ERR_ALREADYREGISTERED"), false; // ERR_ALREADYREGISTERED   // done !
	// if (deq.size() < 5)
	// 	return send_error("ERR_NEEDMOREPARAMS", "USER"), false; // ERR_NEEDMOREPARAMS   // done !
	// if (deq[4][0] != ':')
	// 	return send_message("ERROR SYNTAX"), false; // ERR_chars
	// deq.pop_front();
	// username = deq.front();
	// for (size_t i = 0; i < 3; i++)
	// 	deq.pop_front();
	// deq.front().erase(deq.front().begin());
	// while (1)
	// {
	// 	realname += deq.front();
	// 	deq.pop_front();
	// 	if (deq.empty() == true)
	// 		break ;
	// 	realname += " ";
	// }
	auth |= USERNAME;
	if ((auth & NICKNAME))
	{
		auth |= AUTHENTIFICATED;
		RPL_WELCOME(); // send welcome
	}
	return true;
}


void	client::send_message(string str) const
{
	if (send(fd, str.c_str(), str.size() + 1, 0) == -1)
			send_message(str);
}

void	client::send_error(string str) const
{
	if (str == "ERR_NOSUCHNICK") {
		send_message(::string(":startimes42 401 * "+ nickname +" :No such nick/channel\r\n"));
	}
	else if (str ==  "ERR_NICKNAMEINUSE") {
		send_message(::string(":startimes42 433 * "+nickname+" is already in use.\r\n"));
	}
	else if (str ==  "ERR_NONICKNAMEGIVEN") {
		send_message(::string(":startimes42 431 * :No nickname given\r\n"));
	}
	else if (str ==  "ERR_ERRONEUSNICKNAME") {
		send_message(::string(":startimes42 432 * "+nickname+" :Erroneus nickname\r\n"));
	}
	else if (str == "ERR_ALREADYREGISTERED") {
		send_message(::string(":startimes42 462 * :You may not reregister\r\n"));
	}
	else if (str == "ERR_PASSWDMISMATCH") {
		send_message(::string(":startimes42 464 * :Password incorrect\r\n"));
	}
	else if (str == "ERR_NOSUCHCATEGORY") {
		send_message(::string(":startimes42 479 * :No such category (available categories : bath, feet and armpit)\r\n"));
	}
	else if (str == "ERR_NOTEXTTOSEND") {
		send_message(::string(":startimes42 412 * :No text to send\r\n"));
	}

}

void	client::send_error(string err , string cmd) const
{
	if (err == "ERR_NEEDMOREPARAMS") {
		send_message(::string(":startimes42 461 "+nickname+" "+ cmd +" :Not enough parameters\r\n"));
	}
	else if (err == "ERR_CHANNELISFULL"){
		send_message(::string(":startimes42 471 "+nickname+" "+ cmd +" :Cannot join channel (+l) - Channel is full\r\n"));
	}
	else if (err == "ERR_INVITEONLYCHAN") {
		send_message(::string(":startimes42 473 "+nickname+" "+ cmd +" :Cannot join channel (+i) - You must be invited\r\n"));
	}
	else if (err == "ERR_NOTONCHANNEL") {
		send_message(::string(":startimes42 442 "+nickname+" "+ cmd +" :You're not on that channel\r\n"));
	}
	else if (err == "ERR_USERNOTINCHANNEL") {
		send_message(::string(":startimes42 441 "+nickname+" "+ cmd +" :They aren't on that channel\r\n"));
	}
	else if (err == "ERR_NOTONCHANNEL") {
		send_message(::string(":startimes42 442 "+nickname+" "+ cmd +" :You're not on that channel\r\n"));
	}
	else if (err == "ERR_CHANOPRIVSNEEDED") {
		send_message(::string(":startimes42 482 "+nickname+" "+ cmd +" :You're not channel operator\r\n"));
	}
	else if (err == "ERR_NOSUCHCHANNEL") {
		send_message(::string(":startimes42 403 * "+nickname+" "+ cmd +" :No such channel\r\n"));
	}
	else if (err == "ERR_USERONCHANNEL") {
		send_message(::string(":startimes42 443 * "+nickname+" "+ cmd +" :is already on channel\r\n"));
	}
	else if (err == "ERR_CANNOTSENDTOCHAN") {
		send_message(::string(":startimes42 404 * "+nickname+" "+ cmd +" :Cannot send to channel\r\n"));
	}
	else if (err == "ERR_BADCHANMASK") {
		send_message(::string(":startimes42 476 * "+cmd + " :Bad Channel Mask\r\n"));
	}
	else if (err == "ERR_BADCHANNELKEY") {
		send_message(::string(":startimes42 475 * "+nickname+" "+ cmd +" :Cannot join channel (+k) - bad key\r\n"));
	}
	else if ( err == "ERR_UNKNOWNMODE") {
		send_message(::string(":startimes42 472 * "+nickname+" "+ cmd +" :is unknown mode char to me\r\n"));
	}
}

void	client::RPL_WELCOME(void)
{
	/* e1r11p2.1337.ma 001 a :Welcome to the Internet Relay Network a!~a@127.0.0.1
	:e1r11p2.1337.ma 002 a :Your host is e1r11p2.1337.ma, running version leet-irc 1.0.0
	:e1r11p2.1337.ma 003 a :This server has been started Wed Oct 12 2022
	:e1r11p2.1337.ma 004 a e1r11p2.1337.ma leet-irc 1.0.0 aioOrsw aovimntklbeI
	:e1r11p2.1337.ma 251 a :There are 2 users and 0 services on 1 servers
	:e1r11p2.1337.ma 375 a :- e1r11p2.1337.ma Message of the day -
	:e1r11p2.1337.ma 376 a :End of MOTD command*/

	// string msg = ":irc.example.com 001 " + nickname + " :Welcome to the Example IRC Network, " + nickname + "!" + username + "@localhost" + "\r\n";
	// msg += ":irc.example.com 002 " + nickname + " :Your host is irc.example.com, running version 1.0\r\n";
	// msg += ":irc.example.com 003 " + nickname + " :This server was created 22/03/2023\r\n";
	// msg += ":irc.example.com 004 " + nickname + " irc.example.com 1.0 aovimntklbeI\r\n";
	// msg += ":irc.example.com 251 " + nickname + " :There are 2 users and 0 services on 1 servers\r\n";
	// msg += ":irc.example.com 375 " + nickname + " :- irc.example.com Message of the day -\r\n";
	// msg += ":irc.example.com 372 " + nickname + " :- Welcome to the Example IRC Network, " + nickname + "!" + username + "@localhost" + "\r\n";
	// msg += ":irc.example.com 376 " + nickname + " :End of /MOTD command.\r\n";
	

		std::string msg = ("::startimes42 001 " + nickname + " :welcome to :startimes42\r\n"
			":startimes42 002 " + nickname + " :Your host is :startimes42, running version 1.0\r\n"
			":startimes42 003 " + nickname + " :This server was created 22/03/2023\r\n"
			":startimes42 004 " + nickname + " :startimes42 1.0 - -\r\n"
			":startimes42 372 " + nickname + " welcome to :startimes42\r\n"
			":startimes42 376 " + nickname + " :End of /MOTD command\r\n");

	
	//string msg = ":irc.example.com 001 " + nickname + " :Welcome to the Example IRC Network, " + nickname + "!" + username + "@localhost" + "\r\n";
	send(fd, msg.c_str(), msg.size(), 0);
	// send(fd, "<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]\n",72, 0);
}

void	client::com_sep(std::deque<std::string>& deq, std::vector<std::string>& vec)
{
	deq.pop_front();
	if (deq.empty())
		return ;
	std::stringstream	ss(deq.front());
    std::string			str;
	while (std::getline(ss, str, ','))
        vec.push_back(str);
}

bool client::cmd_KICK(std::deque<std::string>& deq, std::map<std::string,Channel>& channels)
{
	std::string					chan;
	std::vector<std::string>	nicks;
	Channel						*channel;

	if (deq.size() < 3)
		return send_error("ERR_NEEDMOREPARAMS", "KICK"), false; // ERR_NEEDMOREPARAMS   // done !
	chan = deq[1];
	deq.pop_front();
	com_sep(deq, nicks);
	if (channels.find(chan) == channels.end())
		return send_error("ERR_NOSUCHCHANNEL", chan), false; // ERR_NOSUCHCHANNEL    // done !
	channel = &channels[chan];
	// std::vector<int>	&mods = channel->getMods();
	if (channel->vecFind(channel->mods, *this) == channel->mods.end())
		return send_error("ERR_CHANOPRIVSNEEDED", chan), false; // ERR_CHANOPRIVSNEEDED    // done !
	for (size_t i = 0; i < nicks.size(); i++)
	{
		if (channel->kickUser(nicks[i], *this))
		{
			if (channel->vecFind(channel->mods, *this) != channel->mods.end())
				channel->mods.erase(channel->vecFind(channel->mods, *this));
			send_message(::string(":") + nickname + "!" + username + "@localhost" + " KICK " + chan + " " + nicks[i] + "\r\n");
			continue;
		}
	}
	if (channel->users.empty())
		channels.erase(chan);
	return true;
}

bool client::cmd_TOPIC(std::deque<std::string> & deq, std::map<std::string, Channel> & channels)
{
	std::string	chan;
	std::string	topic;

    if (deq.size() < 2)
		return send_error("ERR_NEEDMOREPARAMS", "TOPIC"), false; // ERR_NEEDMOREPARAMS  // done !
	chan = deq[1];
	topic = deq[2];
	if (channels.find(chan) == channels.end())
		return send_error("ERR_NOSUCHCHANNEL", chan), false; // ERR_NOSUCHCHANNEL // done !
	if (channels[chan].vecFind(channels[chan].users, *this) == channels[chan].users.end())
		return send_error("ERR_NOTONCHANNEL", chan), false; // ERR_NOTONCHANNEL // done !
	if (channels[chan].isActive('t') && !channels[chan].isMod(*this)) // done !
		return send_error("ERR_CHANOPRIVSNEEDED", chan), false; // ERR_CHANOPRIVSNEEDED // done !
	channels[chan].setTopic(topic, *this);
	send_message(":"+nickname + "!"+username +"@startimes42 TOPIC "+ chan + " :" + topic + "\r\n");
	return true;
}

bool client::cmd_INVITE(std::deque<std::string> & deq, std::map<int, client>& users, std::map<std::string, Channel> & channels)
{
	std::string	chan;
	std::string	nick;
	if (deq.size() < 3)
		return send_error("ERR_NEEDMOREPARAMS", "INVITE"), false; // ERR_NEEDMOREPARAMS // done !
	chan = deq[2];
	nick = deq[1];
	if (channels.find(chan) == channels.end())
		return send_error("ERR_NOSUCHCHANNEL"), false; // ERR_NOSUCHCHANNEL // done !
	if (channels[chan].vecFind(channels[chan].users, *this) == channels[chan].users.end())
		return send_error("ERR_NOTONCHANNEL"), false; // ERR_NOTONCHANNEL // done !
	if (channels[chan].isActive('i') && !channels[chan].isMod(*this))
		return send_error("ERR_CHANOPRIVSNEEDED"), false; // ERR_CHANOPRIVSNEEDED	// done !
	if (channels[chan].vecFind(channels[chan].users, nick) != channels[chan].users.end())
		return send_error("ERR_USERONCHANNEL", nick), false; // ERR_USERONCHANNEL // done !
	// if (channels[chan].vecFind(channels[chan].users, nick) != channels[chan].users.end())
	if (mapFind(users, nick) != users.end())
	{
		channels[chan].inviteUser((*mapFind(users, nick)).second);
		(*mapFind(users, nick)).second.send_message(":" + nickname + "!"+username+"@startimes42 INVITE " + nick + " " + chan + "\r\n");
	}
	else
		return send_error("ERR_NOSUCHNICK"), false; // ERR_NOSUCHNICK // done !
	return true;
}

// send an anonymous message to a user
// there are 3 categories of messages:
// 1- You have been pinged, your feet smell.
// 2- You have been pinged, you need to take a shower.
// 3- You have been pinged, your armpits smell.

string client::getMessage(string category)
{
	string message;
	if (category == "bath")
		message = "You have been pinged, you need to take a shower.\r\n";
	else if (category == "feet")
		message = "You have been pinged, your feet smell.\r\n";
	else if (category == "armpit")
		message = "You have been pinged, your armpits smell.\r\n";
	else
		return "";
	return message;
}

// send an anonymous message to a user with the name of the bot (Emet)
bool client::cmd_USMELL(std::deque<std::string> & deq, std::map<int, client>& users)
{
	string	nick;
	string	category;
	client	*user;
	string 	msg;

	if (deq.size() < 3)
		return send_error("ERR_NEEDMOREPARAMS", "USMELL"), false; // ERR_NEEDMOREPARAMS // done !
	nick = deq[1];
	category = deq[2];
	if (mapFind(users, nick) == users.end())
		return send_error("ERR_NOSUCHNICK"), false; // ERR_NOSUCHNICK // done !
	user = &(*mapFind(users, nick)).second;
	msg = getMessage(category);
	if (msg != "")
	{
		msg = ":Emet: PRIVMSG " + nick + " :" + msg;
		user->send_message(msg);
		return true;
	}
	else
		return send_error("ERR_NOSUCHCATEGORY"), false; // ERR_NOSUCHCATEGORY // done !

}