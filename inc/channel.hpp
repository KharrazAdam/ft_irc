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

class Client;

class Channel
{
public:
	Channel();
	Channel(std::string, std::string);
	~Channel();

	std::string&	getTitle(void);
	std::string&	GetKey(void);

	bool			add_user(Client& cl);
	void			show_details();
	bool			addMod(Client&);
	
	void	set_i(bool);
	void	set_t(bool);
	void	set_k(bool);
	void	setKey(std::string);
	void	set_l(int);
	std::vector<Client *>::iterator	vecFind(std::vector<Client *>&, Client&);
	std::vector<Client *>::iterator	vecFind(std::vector<Client *>&, std::string&);
	bool							kickUser(std::string &nick, Client &cl, string &);
	bool							isActive(const char &mode);
	bool							setTopic(std::string &topic, Client &cl);
	bool							isMod(Client &cl);
	bool							isUser(Client &cl);
	bool							isInvited(Client &cl);
	bool							inviteUser(Client &cl);
	std::vector<Client *>	users;
	std::vector<Client *>	mods;
	std::vector<Client *>	invited;

private:
	string join_msg(string nickname,string username, string adrs, string);
	string join_msg_exi(string nickname,string username, string adrs, string);
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