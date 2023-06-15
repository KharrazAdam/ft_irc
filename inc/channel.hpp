#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

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

	std::string&	getTitle(void);
	std::string&	GetKey(void);

	bool			add_user(client& cl);
	void			show_details();

	std::vector<client> users;
	std::vector<client> mods;
	std::vector<client>	invited;
	std::vector<client>::iterator	vecFind(std::vector<client>&, client&);
private:

	std::string	title;
	std::string	key;
	std::string	topic;

	int		l;
	bool	k;
	bool	i;
};
#include "client.hpp"
#endif