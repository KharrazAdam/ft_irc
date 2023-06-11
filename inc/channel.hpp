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
	~Channel();
	bool	add_user(client&);
private:
	std::vector<int> mods;
	std::map<int, std::string&> users;
	std::string title;
	std::string key;
};
#include "client.hpp"
#endif