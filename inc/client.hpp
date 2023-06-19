# ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <sstream>

# include <deque>
# include <map>
# include <vector>

# include <sys/socket.h>


# define PASSWORD 1
# define USERNAME 2 // 0b10
# define NICKNAME 4 // 0b100
# define AUTHENTIFICATED 8
# define REGISTRED 16

using namespace std;

class Channel;
class client
{
private:
	// <nick!user@host>
	int			fd;
	short		auth;
	std::string	username;
	std::string	nickname;
	std::string	realname;
	std::string	addr;

	std::vector<std::string> channels;

	void	SetAuth(int);
	void	SetUser(std::string);
	void	SetNick(std::string);
public:
	client(int);
	client();
	~client();
	int				ShowAuth(void);
	std::string&		getNick(void);
	std::string&		getUsername(void);

	int				getFd(void);
	std::map<int, client>::iterator	mapFind(std::map<int, client>&, std::string&);
	// <-----commands------->
	bool	cmd_PASS(std::deque<std::string>&, std::string&);

	bool	cmd_NICK(std::deque<std::string>&, std::map<int, client>& cl);

	bool	cmd_USER(std::deque<std::string>&);

	bool	cmd_JOIN(std::deque<std::string>&, std::map<std::string, Channel>&);
	void	com_sep(std::deque<std::string>&, std::vector<std::string>&);

	bool	cmd_NOTICE(std::deque<std::string>&, std::map<int, client>& cl, std::map<std::string, Channel>&);
	bool	noticeCl(std::map<int, client>&, std::string&, std::string&);
	bool	noticeCh(std::map<std::string, Channel>&, std::string&, std::string&);

	bool	cmd_PRIVMSG(std::deque<std::string>&, std::map<int, client>& cl, std::map<std::string, Channel>&);
	bool	msgCl(std::map<int, client>&, std::string&, std::string&);
	bool	msgCh(std::map<std::string, Channel>&, std::string&, std::string&);
	bool	print_pvmsg(int fd, std::string& message, std::string& receiver,bool isChannel);

	bool	cmd_MODE(std::deque<std::string>&, std::map<int, client>& cl, std::map<std::string, Channel>&);
	void	flag_i(Channel&, bool);
	void	flag_t(Channel&, bool);
	void	flag_k(Channel&, bool, std::deque<std::string>&);
	void	flag_o(Channel&, bool, client&);

	bool	cmd_KICK(std::deque<std::string>&, std::map<std::string, Channel>&);

	bool	cmd_TOPIC(std::deque<std::string>&, std::map<std::string, Channel>&);
	
	bool	cmd_INVITE(std::deque<std::string>&, std::map<int, client>&, std::map<std::string, Channel>&);

	bool	cmd_USMELL(std::deque<std::string>&, std::map<int, client>&);
	// ....
	// bonus
	bool	cmd_SHOW(std::deque<std::string>&, std::map<std::string, Channel>&);
	string	getMessage(string category);

	
	// <-----errors--------->
	void	send_error(string str, string cmd) const;
	void	send_error(string str) const;
	void	send_message(string str) const;
	void	RPL_WELCOME(void);
};
#include "channel.hpp"
#endif