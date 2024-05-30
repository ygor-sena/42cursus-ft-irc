/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:26:17 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/30 20:41:21 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel()
{
	_key = "";
	_name = "";
	_topic = "";
	_limit = -1;
	_created_at = "";
	_has_key = false;
}

Channel::Channel(std::string name)
{
	_key = "";
	_topic = "";
	_limit = -1;
	_name = name;
	_created_at = "";
	_has_key = false;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel()
{
	this->_clients.clear();
	this->_operator_clients.clear();
	return ;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/**
 * @brief Getter for the name of the channel
 *
 * @param void
 *
 * @return std::string
 *
 * @note This function returns the name of the channel
 *
 *	Example:
 *		Channel channel("channel");
 *		std::cout << channel.get_name() << std::endl;
 *		// Output: channel
*
 * @return std::string
 */
std::string Channel::get_name(void) const
{
	return _name;
}

/**
 * @brief Getter for the topic restriction of the channel
 *
 * @return true
 * @return false
 */
bool Channel::get_topic_restriction(void) const
{
	return _topic_restriction;
}

/**
 * @brief Getter for the clients that are channel members
 *
 * @return std::vector<Client *>
 */
std::vector<Client *> Channel::get_operator_clients(void)
{
	return this->_operator_clients;
}

/**
 * @brief Getter for the clients that are channel members
 *
 * @return std::vector<Client *>
 */
std::vector<Client *> Channel::get_channel_clients(void)
{
	return this->_clients;
}

/**
 * @brief Getter for the clients that are channel members
 *
 * @return std::string
 */
std::string Channel::get_client_names(void) const
{
	std::string names;
	for (std::vector<Client *>::const_iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		names += (*it)->get_nickname();
		names += " ";
	}
	return names;
}

/**
 * @brief Getter for the number of clients in the channel
 *
 * @return int
 */
int Channel::get_clients_size(void) const
{
	return this->_clients.size();
}

/**
 * @brief Getter for the topic of the channel
 *
 * @return std::string
 */
std::string Channel::get_topic(void) const
{
	return _topic;
}

/**
 * @brief Getter for the key of the channel
 *
 * @return std::string
 */
std::string Channel::get_channel_key(void) const
{
	return _key;
}


/**
 * @brief  Getter for the limit of the channel
 *
 * @param client
 */
void Channel::set_channel_operator(Client *client)
{
	client->set_is_operator(true);
	this->_operator_clients.push_back(client);
	return ;
}

/*
 * ------------------------------- SETTERS --------------------------------
*/

/**
 * @brief Setter for the topic of the channel
 *
 * @param topic
 */
void Channel::set_topic(std::string topic)
{
	_topic = topic;
}

/**
 * @brief  Setter for the limit of the channel
 *
 * @param limit
 */
void Channel::set_limit(int limit)
{
	_limit = limit;
}

/**
 * @brief  Setter for the invite only of the channel
 *
 */
void Channel::set_invite_only()
{
	_invite_only = true;
}

/**
 * @brief  Setter for the topic restriction of the channel
 *
 */
void Channel::set_topic_restriction()
{
	_topic_restriction = true;
}

/**
 * @brief  Setter for the key of the channel
 *
 * @param key
 */
void Channel::set_key(std::string key)
{
	_key = key;
}

/*
** ------------------------------- REMOVERS --------------------------------
*/
/**
 * @brief  Remove the limit of the channel
 *
 */
void Channel::remove_limit(void)
{
	_limit = -1;
}

/**
 * @brief  Remove the key of the channel
 *
 *
 * */
void Channel::remove_key(void)
{
	_key = "";
}

/**
 * @brief  Remove the topic restriction of the channel
 *
 *  @_topic_restriction are the topic restrictions of a channel.
 *  If true, only channel operators can change the topic.
 */
void Channel::remove_topic_restriction(void)
{
	_topic_restriction = false;
}

/**
 * @brief  Remove the invite only of the channel
 *
 */
void Channel::remove_invite_only(void)
{
	_invite_only = false;
}

/**
 * @brief  Remove the channel operator
 *
 * @param client
 */
void Channel::remove_channel_operator(Client *client)
{
	client->set_is_operator(false);
	for (std::vector<Client *>::iterator it = this->_operator_clients.begin(); it != this->_operator_clients.end(); ++it)
	{
		if((*it)->get_nickname() == client->get_nickname())
		{
			this->_operator_clients.erase(it);
			return ;
		}
	}
}

/**
 * @brief  Remove the channel client
 *
 * @param client
 */
void Channel::remove_channel_client(Client *client)
{
	for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end();)
    {
        if ((*it)->get_nickname() == client->get_nickname())
        {
            std::cout << (*it)->get_nickname() << std::endl;
            it = this->_clients.erase(it);
            break ;
        }
        else
            ++it;
    }
	return ;
}

/*
** --------------------------------- OTHERS ---------------------------------
*/

/**
 * @brief  Check if the client is in the channel. STATUS and checker functions
 *
 * @param client
 *
 * @return true
 * @return false
 */
bool Channel::has_client(Client *client)
{
	for (std::vector<Client *>::const_iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if ((*it)->get_nickname() == client->get_nickname())
			return true;
	}
	return false;
}


/**
 * @brief  Check if the channel has a key
 *
 * @return true
 * @return false
 */
bool Channel::has_key(void) const
{
	return _key != "";
}

/**
 * @brief  Check if the client is a channel  by `fd`
 *
 * @param fd
 *
 * @return true
 * @return false
 */
bool Channel::is_channel_operator(const int fd)
{
	for (std::vector<Client *>::iterator it = _operator_clients.begin(); it != this->_operator_clients.end(); ++it)
	{
		if ((*it)->get_fd() == fd)
			return true;
	}
	return false;
}

/*
 * Member methods for CHANNEL MANAGEMENT
*/


/**
 * @brief Invites a client to join the channel.
 *
 * This method adds the specified client to the list of clients in the channel.
 *
 * @param client A pointer to the client to be invited.
 */
void Channel::invite(Client *client)
{
	_clients.push_back(client);
	return ;
}


/**
 * @brief Adds a client to the channel.
 *
 * This method adds the specified client to the channel's list of clients.
 *
 * @param client A pointer to the client to be added.
 */
void Channel::join(Client *client)
{
	_clients.push_back(client);
	return ;
}

/**
 * @brief Kicks a client from the channel.
 *
 * This method removes the client from the list of invited clients,
 * removes the client as a channel operator, and removes the client
 * from the list of clients in the channel.
 *
 * @param client A pointer to the client to be kicked.
 */
void Channel::kick(Client *client)
{
	client->remove_channel_invited(this->get_name());
	remove_channel_operator(client);
	remove_channel_client(client);
}

/**
 * @brief Removes a client from the channel.
 *
 * This method removes the specified client from the channel by removing them as a channel operator and as a channel client.
 *
 * @param client A pointer to the client to be removed from the channel.
 */
void Channel::part(Client *client)
{
	remove_channel_operator(client);
	remove_channel_client(client);
}

/**
 * @brief Broadcasts a message to all clients in the channel, excluding the sender.
 *
 * This method iterates over the list of clients in the channel and calls the `broadcast` method
 * on each client, passing the sender, target, and message as arguments. The `broadcast` method
 * is called for both regular clients and operator clients.
 *
 * @param sender A pointer to the client who sent the message.
 * @param target The target of the message.
 * @param message The message to be broadcasted.
 */
void Channel::broadcast(Client *sender, std::string target, std::string message)
{
	for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++) {
		if (*it == sender)
			continue ;
		(*it)->broadcast(sender, target, message);
	}
	for (std::vector<Client *>::iterator it = this->_operator_clients.begin(); it != this->_operator_clients.end(); it++) {
		if (*it == sender)
			continue ;
		(*it)->broadcast(sender, target, message);
	}
	return ;
}

/**
 * @brief Checks if a client with the given nickname is in the channel.
 *
 * @param nickname The nickname of the client to check.
 * @return true if the client is in the channel, false otherwise.
 */
bool Channel::is_client_in_channel(std::string nickname)
{
	for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if((*it)->get_nickname() == nickname)
			return true;
	}
	return false;
}

/**
 * @brief Checks if the channel is full.
 *
 * This method checks if the channel has reached its maximum capacity.
 * If the channel has no limit (-1), it is considered not full.
 *
 * @return true if the channel is full, false otherwise.
 */
bool Channel::is_channel_full(void) const
{
	if (_limit == -1)
		return false;
	if (_clients.size() >= static_cast<size_t>(_limit))
		return true;
	return false;
}

/**
 * @brief Check if the channel is invite-only.
 *
 * @return true if the channel is invite-only, false otherwise.
 */
bool Channel::is_channel_invite_only(void) const
{
	return _invite_only;
}
