#include "ircserv.hpp"
# define TIMEOUT 1000

ircserv::ircserv(){};
ircserv::~ircserv(){};

string makeUppercase(const string& str)
{
	string result = str;
	for (string::iterator it = result.begin(); it != result.end(); ++it)
	    *it = toupper(*it);
	return result;
}

/**
 * @brief checks if all the str's elemnts are digits
 * @return true if str is number otherwise false 
*/
bool	ircserv::is_num(string str)
{
	size_t size = str.size();
	for (size_t i = 0; i < size; i++)
	{
		if (isdigit(str[i]) == false)
			return (false);
	}
	return (true);
}

/**
 * @param av is the second command line's arg
 * @brief checks if the param is number and checks for it's range 1024 -> 65535
 * @return true if valid or false otherwise
*/
bool	ircserv::ircserv_port(const char *av, const char* pass)
{
	stringstream ss(av);

	if (!pass[0] || string(pass).find_first_of(" \t\n\r") != string::npos)
		return (cerr << "Error: invalid password" << endl, false);
	
	if (ss.str().size() > 5 || is_num(ss.str()) == false)
		return (cerr << "Error: invalid port" << endl, false);
	ss >> port;

	if (port >= 1024 && port <= 65535)
		return (true);
	return cerr << "Error: port out of range: 1024 -> 65535" << endl, false;
}

/**
 * @param addr the struct used to store socket's info
 * @param sock is the file descriptor of server's socket
 * @brief binds the socket to the provided port and the host's IP adress
 * @return false if binding faild. otherwise true
*/
bool	ircserv::ircserv_bind(sockaddr_in *addr, int sock)
{
	bzero(addr, sizeof(sockaddr_in));
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port); // host to network short: if it's small endian converts it to big endian	
	addr->sin_len = sizeof(sockaddr_in);
	int opt = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		return cerr << "Error: setsockopt()" << endl, false;
	if (bind(sock, (sockaddr *)addr, sizeof(sockaddr_in)) == -1)
		return cerr << "Error: bind()" << endl, false;
	return (true);
}

bool	ircserv::ircserv_quit(pollfd& Ps)
{
	std::map<std::string, Channel>::iterator	it;

	for (it = channels.begin(); it != channels.end(); it++)
	{
		// if invited
		if ((*it).second.isInvited(user[Ps.fd]))
			(*it).second.invited.erase((*it).second.vecFind((*it).second.invited, user[Ps.fd].getNick()));
		// if moderator
		if ((*it).second.isMod(user[Ps.fd]))
		{
			(*it).second.mods.erase((*it).second.vecFind((*it).second.mods, user[Ps.fd].getNick()));
			(*it).second.users.erase((*it).second.vecFind((*it).second.users, user[Ps.fd].getNick()));
			if ((*it).second.users.size() == 0)
				(*it).second.invited.clear(); // close channel
			if ((*it).second.mods.size() == 0 && (*it).second.users.size() > 0)
				(*it).second.mods.push_back((*it).second.users.front()); // close channel
		}
		//if user
		else if ((*it).second.isUser(user[Ps.fd]))
		{
			for (vector<Client *>::iterator ito = (*it).second.users.begin(); ito != (*it).second.users.end(); ito++)
				(*ito)->send_message(::string(":"+user[Ps.fd].getNick() + " PART " + (*it).second.getTitle() + " : Bye for now\n\r"));
			(*it).second.users.erase((*it).second.vecFind((*it).second.users, user[Ps.fd].getNick()));
		}
	}
	return true;
}

/**
 * @param pollfd structur that holds client fd and events that can be monitored on the fd
 * @brief store message in str to be used later
 * @return false if syscall recv() faild
*/
bool	ircserv::ircserv_msg(pollfd& Ps, string& str, int *num)
{
	int			rs;
	char		buffer[1024];
	
	str.clear();
	bzero(buffer, 1024);
	while (1)
    {
        rs = recv(Ps.fd, buffer, 1024, 0);
        if (rs == -1 || rs == 0)
            break ;
        str.append(buffer, rs);
        if (str.find("\n") == ::string::npos)
            continue ;
        else
            break;
    }
	if (rs == -1)
		return cerr << "Error: recv()" << endl, false;
	if (rs == 0)
	{
		ircserv_quit(Ps);
		cerr << "client disconnnected" << endl;
		user.erase(Ps.fd);
		close(Ps.fd);
		(void)(num);
		Ps.fd = -1;
		return false;
	}
	cout << "|" << str << "|" << endl;
	return (true);
}

// int countNewlines(const std::string& str) {
//     int newlineCount = 0;
// 	int i = 0;
//     while (str[i] != '\0') {
//         if (str[i] == '\n')
//             newlineCount++;
// 		i++;
//     }
//     return newlineCount;
// }


/**
 * @param deq deque to be filled
 * @param str strint containig the cmd and params
 * @brief convert cmd from a string to deque
*/
bool	ircserv::ircserv_cmd(deque<string>& deq, string str)
{
	stringstream	ss(str);
	string			splited;

	while (ss >> splited)
		deq.push_back(splited);
	if (deq.empty() == true)
		return false;
	return true;
}

void	nl_sep(std::vector<std::string>& vec, std::string& str)
{
	std::stringstream	ss(str);

	while (std::getline(ss, str, '\n'))
        vec.push_back(str);
}

char	ircserv::ircserv_auth(pollfd& Ps, std::string& str)
{
	size_t i = 0;
	std::deque<std::string> deq;
	std::vector<std::string> vec;
	nl_sep(vec, str);

	while (i < vec.size())
	{
		deq.clear();
		string line = vec[i];
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
		ircserv_cmd(deq, line);	
		deq[0] = makeUppercase(deq.front());
		if (deq.front() == "PASS")
		{
			if (user[Ps.fd].cmd_PASS(deq, password) == false)
				return false;
		}
		if ((user[Ps.fd].ShowAuth() & PASSWORD) == 0)
		{
			i = 0;
			break ;
		}
		if (deq.front() == "NICK")
		{
			if (user[Ps.fd].cmd_NICK(deq, user) == false)
				return false;
		}
		if (deq.front() == "USER")
		{
			if (user[Ps.fd].cmd_USER(deq) == false)
				return false;
		}
		i++;
	}
	if (i == 0)
		return 2;
	return 1;
}

bool	ircserv::ircserv_receiv(pollfd& Ps, int *num)
{
	string str;
	deque<string> deq;

	if (ircserv_msg(Ps, str, num) == false)
		return false;
	if (ircserv_cmd(deq, str) == false)
		return false;
	if (ircserv_auth(Ps, str) == false)
		return false;
	deq[0] = makeUppercase(deq.front());
	if ((user[Ps.fd].ShowAuth() & AUTHENTIFICATED) == 0)
		return false;
	cout << "user authentificated == "<< deq.front() << endl;
	deq[0] = makeUppercase(deq.front());
	cout <<user[Ps.fd].getNick() <<" user has ben connected !!" << "" <<endl;
	if (deq.front() == "JOIN")
	{
		cout << "im here >>> in join " << endl;
		return user[Ps.fd].cmd_JOIN(deq, channels);
	}
	else if (deq.front() == "SHOW")
		return user[Ps.fd].cmd_SHOW(deq, channels);
	else if (deq.front() == "PRIVMSG")
		return user[Ps.fd].cmd_PRIVMSG(deq, user, channels);
	else if (deq.front() == "NOTICE")
		return user[Ps.fd].cmd_NOTICE(deq, user, channels);
	else if (deq.front() == "MODE")
		return user[Ps.fd].cmd_MODE(deq, user, channels);
	else if (deq.front() == "KICK")
		return user[Ps.fd].cmd_KICK(deq, channels);
	else if (deq.front() == "INVITE")
		return user[Ps.fd].cmd_INVITE(deq, user, channels);
	else if (deq.front() == "TOPIC")
		return user[Ps.fd].cmd_TOPIC(deq, channels);
	else if (deq.front() == "USMELL")
		return user[Ps.fd].cmd_USMELL(deq, user);
	return true;
}

bool	ircserv::ircserv_connect(pollfd& Ps, int sock, int *num)
{
	int			client;
	struct sockaddr_in addr;

	client = accept(sock, (struct sockaddr *)&addr, (socklen_t *)&addr);
	if (client == -1)
		return cerr << "Error: accept()" << endl, false;
	else
	{
		Ps.fd = client;
		Ps.events = POLLIN;
		user[Ps.fd] = ::Client(Ps.fd);
		user[Ps.fd].setAddr(inet_ntoa(addr.sin_addr));
		// user[Ps.fd](::client(Ps.fd));
		(*num)++;
	}
	return true;
}

/**
 * @param socket the srver's socket fd
 * @brief accepts new connctions and receive messages from clients
 * @return false if syscall fail otherwise the loop keeps looping
 * @bug when deconnect a client. its memory remains unused!!!
*/
bool	ircserv::ircserv_serv(int sock)
{
	pollfd		Ps[MAX_POLLFD];
	int			num;
	int			rs;

	num = 1;
	bzero(Ps, MAX_POLLFD);
	Ps[0].fd = sock;
	Ps[0].events = POLLIN;
	while (1)
	{
		rs = poll(Ps, MAX_POLLFD, -1);
		if (rs == -1)
			return cerr << "Error: poll()" << endl, false;
		for (int i = 0; i < num; i++)
		{
			if (Ps[i].revents & POLLIN)
			{
				if (Ps[i].fd == sock)
				{
					for (rs = 0; rs < num; rs++)
					{
						if (Ps[rs].fd == -1)
							break ;
					}
					ircserv_connect(Ps[rs],  sock, &num);
				}
				else
					ircserv_receiv(Ps[i], &num);
			}
		}
	}
	return (true);
}

/**
 * @brief binds the socket, Listens, and receiving connections
 * @return false if any inconvenience occurs 
*/
bool	ircserv::ircserv_run(void)
{
	int				sock;
	sockaddr_in		addr;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		return cerr << "Error: socket()" << endl, false;
	if (ircserv_bind(&addr, sock) == false)
		return close(sock), false;
	cout << "running on the address: " << inet_ntoa(addr.sin_addr) << endl;
	if (listen(sock, SOMAXCONN) == -1)
		return cerr << "Error: listen()" << endl, close(sock), false;	
	if (ircserv_serv(sock) == false)
		return close(sock), false;
	return (close(sock), true);
}

bool	ircserv::ircserv_start(char **av)
{
	if (ircserv_port(av[1], av[2]) == false)
		return false;
	password = av[2];
	if (ircserv_run() == false)
		return false;
	return true;
};
