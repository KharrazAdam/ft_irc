#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

// channel modes
// +t - only ops can change topic
// +i - only ops can invite users
// +l - limit on number of users
// +k - password
// +o - op

class client;

class Channel
{
public:
	Channel();
	Channel(std::string, std::string);
	~Channel();
	bool			add_user(client& cl);
	std::string&	GetKey(void);
	void			show_details();
	std::map<std::string, int>&	getUsers(void);
	std::vector<int>&	getMods(void);
	bool				kickUser(std::string &nick, client &cl);
	bool				addMod(int fd);
	bool				setTopic(std::string &topic, client &cl);
	bool				isMod(int fd);
	bool				isUser(std::string &nick);
	bool				isActive(const char &mode);
	bool				inviteUser(std::string &nick, client &cl);
private:
	std::vector<int> 			mods;
	std::map<std::string, int>	users;
	std::set<std::string>		invited;

	std::string	title;
	std::string	key;
	std::string	topic;
	

	int		l;
	bool	k;
	bool	i;
	bool	t;
};
#include "client.hpp"
#endif