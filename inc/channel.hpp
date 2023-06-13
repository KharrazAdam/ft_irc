#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

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
	bool			kickUser(std::string &nick, client &cl);
	bool			addMod(int fd);
private:
	std::vector<int> 			mods;
	std::map<std::string, int> users;
	std::map<std::string, int> invited;

	std::string	title;
	std::string	key;
	std::string	topic;
	

	size_t		l;
	bool	k;
	bool	i;
};
#include "client.hpp"
#endif