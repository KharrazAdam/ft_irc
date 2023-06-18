#include "ircserv.hpp"

int	main(int ac, char **av)
{
	ircserv irc;

	if (ac != 3)
		return std::cerr << "Error: \"./ircserv <port> <password>\"" , 1;
	if (irc.ircserv_start(av) == false)
		return (1);
	return (0);
}
