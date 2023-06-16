
bool Channel::inviteUser(std::string &nick)
{
	invited.insert(nick);
	return true;
}