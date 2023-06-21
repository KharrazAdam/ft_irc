#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <set>

// channel modes
// +t - only ops can change topic
// +i - only ops can invite users
// +l - limit on number of users
// +k - password
// +o - op

using namespace std;

class client;

class Channel
{
public:
	Channel();
	Channel(std::string, std::string);
	~Channel();

	std::string&	getTitle(void);
	std::string&	GetKey(void);

	bool			add_user(client& cl);
	void			show_details();
	bool			addMod(client&);
	
	void	set_i(bool);
	void	set_t(bool);
	void	set_k(bool);
	void	setKey(std::string);
	std::vector<client *>::iterator	vecFind(std::vector<client *>&, client&);
	std::vector<client *>::iterator	vecFind(std::vector<client *>&, std::string&);
	bool							kickUser(std::string &nick, client &cl);
	bool							isActive(const char &mode);
	bool							setTopic(std::string &topic, client &cl);
	bool							isMod(client &cl);
	bool							isUser(client &cl);
	bool							isInvited(client &cl);
	bool							inviteUser(client &cl);
	std::vector<client *>	users;
	std::vector<client *>	mods;
	std::vector<client *>	invited;

private:
	string join_msg(string nickname,string username, string);
	string join_msg_exi(string nickname,string username, string);
	// std::map<std::string, int>&	getUsers(void);
	// std::vector<int>&	getMods(void);
	// std::vector<int> 			mods;
	// std::map<std::string, int>	users;
	// std::set<std::string>		invited;

	std::string	title;
	std::string	key;
	std::string	topic;

	int		l;
	bool	k;
	bool	i;
	bool	t;

	void send_msg(string msg);
};
#include "client.hpp"
#endif