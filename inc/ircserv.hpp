# ifndef IRCSERV_HPP
# define IRCSERV_HPP

# include <sstream>
# include <unistd.h>
# include <sstream>

# include <sys/socket.h>
# include <poll.h>
# include <netinet/in.h>
# include <arpa/inet.h>

# include <vector>
# include <map>
# include <deque>

# include "client.hpp"

# define TCP_MSS 1460
# define MAX_POLLFD 200

using namespace std;


class ircserv
{
private:
	int								port;
	std::string						password;
	std::map<int, Client>			user;
	std::map<std::string, Channel>	channels;

	std::vector<Client>::iterator	vecFind(std::vector<Client>&, int&);

	bool	is_num(std::string);
	bool	ircserv_port(const char*, const char *);
	bool	ircserv_run(void);
	bool	ircserv_bind(sockaddr_in *addr, int sock);
	bool	ircserv_serv(int sock);
	bool	ircserv_receiv(pollfd& Ps, int *num);
	bool	ircserv_connect(pollfd& Ps, int sock, int *num);
	bool	ircserv_msg(pollfd& Ps, std::string& str, int *num);
	bool	ircserv_cmd(std::deque<std::string>&, std::string str);
	char	ircserv_auth(pollfd& Ps, std::string&);
	bool	ircserv_quit(pollfd& Ps);
	//<------ a reviser--------->

public:
	bool	ircserv_start(char **);
	ircserv();
	~ircserv();
};

#endif