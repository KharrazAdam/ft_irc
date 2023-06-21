# include "client.hpp"
# include "channel.hpp"

bool	Client::cmd_SHOW(std::deque<std::string>& deq, std::map<std::string, Channel>& chan)
{
	deq.pop_front();
	
	if (chan.find(deq.front()) != chan.end())
		chan[deq.front()].show_details();
	return true;
}

void	Channel::show_details()
{
	std::vector<Client *>::iterator it = users.begin();

	std::cout << "title: " << title << std::endl;
	std::cout << "topic: " << topic << std::endl;
	std::cout << "key: " << key << std::endl; 
	std::cout << "users: "; 
	for (it = users.begin(); it != users.end(); it++)
		std::cout << (*it)->getNick() << " ";
	std::cout << std::endl;
	
	std::cout << "mods are: "; 
	for (it = mods.begin(); it != mods.end(); it++)
		std::cout << (*it)->getNick() << " ";
	std::cout << std::endl;
	
	std::cout << "invited are: "; 
	for (it = invited.begin(); it != invited.end(); it++)
		std::cout << (*it)->getNick() << " ";
	std::cout << std::endl;
	
	// todo: rmove this
	(void)t;
	return ;
}
