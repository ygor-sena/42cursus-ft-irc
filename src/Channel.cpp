/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:26:17 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/31 10:10:47 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel()
{
	_limit = -1;
	_has_key = false;
	_invite_only = false;
	_topic_restriction = false;
	_key = "";
	_name = "";
	_topic = "";
	_created_at = "";
	_clients = std::vector<Client*>();
	_operator_clients = std::vector<Client*>();
}

Channel::Channel(std::string name)
{
	_limit = -1;
	_has_key = false;
	_invite_only = false;
	_topic_restriction = false;
	_key = "";
	_name = name;
	_topic = "";
	_created_at = "";
	_clients = std::vector<Client*>();
	_operator_clients = std::vector<Client*>();
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel()
{
	_clients.clear();
	_operator_clients.clear();
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/**
 * @brief Get the name of the channel.
 *
 * @return std::string The name of the channel.
 */
std::string Channel::get_name(void) const { return _name; }

/**
 * @brief Get the topic restriction flag of the channel.
 *
 * @return bool The topic restriction flag.
 */
bool Channel::get_topic_restriction(void) const { return _topic_restriction; }

/**
 * @brief Get the operator clients of the channel.
 *
 * @return std::vector<Client*> The operator clients of the channel.
 */
std::vector<Client*> Channel::get_operator_clients(void)
{
	return this->_operator_clients;
}

/**
 * @brief Get the clients in the channel.
 *
 * @return std::vector<Client*> The vector of clients in the channel.
 */
std::vector<Client*> Channel::get_channel_clients(void)
{
	return this->_clients;
}

/**
 * @brief Get the names of all clients in the channel.
 *
 * This function returns a string containing the names of all clients in the
 * channel, separated by a space.
 *
 * @return std::string The names of all clients in the channel.
 */
std::string Channel::get_client_names(void) const
{
	std::string names;
	for (std::vector<Client*>::const_iterator it = this->_clients.begin();
		 it != this->_clients.end();
		 ++it)
	{
		names += (*it)->get_nickname();
		names += " ";
	}
	return names;
}

/**
 * @brief Get the number of clients in the channel.
 *
 * @return The number of clients in the channel.
 */
int Channel::get_clients_size(void) const { return this->_clients.size(); }

/**
 * @brief Get the topic of the channel.
 *
 * @return std::string The topic of the channel.
 */
std::string Channel::get_topic(void) const { return _topic; }

/**
 * @brief Get the channel key.
 *
 * This function returns the key of the channel.
 *
 * @return std::string The channel key.
 */
std::string Channel::get_channel_key(void) const { return _key; }

/**
 * @brief Sets a client as a channel operator.
 *
 * This function sets the given client as a channel operator by setting its
 * "is_operator" flag to true and adding it to the list of operator clients.
 *
 * @param client A pointer to the client to be set as a channel operator.
 */
void Channel::set_channel_operator(Client* client)
{
	client->set_is_operator(true);
	this->_operator_clients.push_back(client);
	return;
}

/**
 * @brief Set the topic of the channel.
 *
 * This function sets the topic of the channel to the specified topic.
 *
 * @param topic The new topic of the channel.
 */
void Channel::set_topic(std::string topic) { _topic = topic; }

/**
 * @brief Set the limit of the channel.
 *
 * This function sets the limit of the channel, which determines the maximum
 * number of users that can join the channel.
 *
 * @param limit The limit of the channel.
 */
void Channel::set_limit(int limit) { _limit = limit; }

/**
 * @brief Sets the invite-only flag for the channel.
 *
 * This function sets the invite-only flag for the channel. When the invite-only
 * flag is set to true, only users who have been invited can join the channel.
 */
void Channel::set_invite_only() { _invite_only = true; }

/**
 * @brief Sets the topic restriction flag for the channel.
 *
 * This function sets the topic restriction flag for the channel. When the flag
 * is set to true, only channel operators will be able to change the channel's
 * topic.
 */
void Channel::set_topic_restriction() { _topic_restriction = true; }

/**
 * @brief Sets the key for the channel.
 *
 * This function sets the key for the channel. The key is used to restrict
 * access to the channel.
 *
 * @param key The key to set for the channel.
 */
void Channel::set_key(std::string key) { _key = key; }

/**
 * @brief Removes the limit of the channel.
 *
 * This function sets the limit of the channel to -1, indicating that there is
 * no limit.
 */
void Channel::remove_limit(void) { _limit = -1; }

/**
 * @brief Removes the key of the channel.
 *
 * This function sets the key of the channel to an empty string, effectively
 * removing the key.
 */
void Channel::remove_key(void) { _key = ""; }

/**
 * @brief Removes the topic restriction for the channel.
 *
 * This function sets the topic restriction flag to false, allowing any user to
 * change the channel's topic.
 */
void Channel::remove_topic_restriction(void) { _topic_restriction = false; }

/**
 * @brief Removes the invite-only flag from the channel.
 *
 * This function sets the `_invite_only` member variable of the Channel class to
 * false, indicating that the channel is no longer invite-only.
 */
void Channel::remove_invite_only(void) { _invite_only = false; }

/**
 * @brief Removes a channel operator from the channel.
 *
 * This function removes the specified client from the list of channel
 * operators. It sets the client's "is_operator" flag to false and removes it
 * from the "_operator_clients" vector.
 *
 * @param client A pointer to the client to be removed as a channel operator.
 */
void Channel::remove_channel_operator(Client* client)
{
	client->set_is_operator(false);
	for (std::vector<Client*>::iterator it = this->_operator_clients.begin();
		 it != this->_operator_clients.end();
		 ++it)
	{
		if ((*it)->get_nickname() == client->get_nickname())
		{
			this->_operator_clients.erase(it);
			return;
		}
	}
}

/**
 * @brief Removes a client from the channel.
 *
 * This function removes the specified client from the channel's list of
 * clients.
 *
 * @param client A pointer to the client to be removed.
 */
void Channel::remove_channel_client(Client* client)
{
	for (std::vector<Client*>::iterator it = this->_clients.begin();
		 it != this->_clients.end();)
	{
		if ((*it)->get_nickname() == client->get_nickname())
		{
			std::cout << (*it)->get_nickname() << std::endl;
			it = this->_clients.erase(it);
			break;
		}
		else
			++it;
	}
	return;
}

/*
** --------------------------------- OTHERS ---------------------------------
*/

// Channel STATUS and checker functions
/**
 * @brief Checks if a given client is present in the channel.
 *
 * @param client A pointer to the client to be checked.
 * @return true if the client is present in the channel, false otherwise.
 */
bool Channel::has_client(Client* client)
{
	for (std::vector<Client*>::const_iterator it = this->_clients.begin();
		 it != this->_clients.end();
		 ++it)
	{
		if ((*it)->get_nickname() == client->get_nickname())
			return true;
	}
	return false;
}

/**
 * @brief Checks if the channel has a key.
 *
 * This function checks if the channel has a key assigned to it.
 *
 * @return true if the channel has a key, false otherwise.
 */
bool Channel::has_key(void) const { return _key != ""; }

/**
 * @brief Checks if a given nickname belongs to a channel operator.
 *
 * @param nickname The nickname to check.
 * @return True if the nickname belongs to a channel operator, false otherwise.
 */
bool Channel::is_channel_operator(std::string nickname)
{
	for (std::vector<Client*>::iterator it = _operator_clients.begin();
		 it != this->_operator_clients.end();
		 ++it)
	{
		if ((*it)->get_nickname() == nickname)
			return true;
	}
	return false;
}

/**
 * @brief Checks if a client with the given file descriptor is a channel
 * operator.
 *
 * @param fd The file descriptor of the client.
 * @return true if the client is a channel operator, false otherwise.
 */
bool Channel::is_channel_operator(const int fd)
{
	for (std::vector<Client*>::iterator it = _operator_clients.begin();
		 it != this->_operator_clients.end();
		 ++it)
	{
		if ((*it)->get_fd() == fd)
			return true;
	}
	return false;
}

// Member functions for CHANNEL MANAGEMENT
/**
 * @brief Invites a client to the channel.
 *
 * This function adds the specified client to the list of clients in the
 * channel.
 *
 * @param client A pointer to the client to be invited.
 */
void Channel::invite(Client* client)
{
	_clients.push_back(client);
	return;
}

/**
 * @brief Adds a client to the channel.
 *
 * This function adds the specified client to the channel's list of clients.
 *
 * @param client A pointer to the client to be added.
 */
void Channel::join(Client* client)
{
	_clients.push_back(client);
	return;
}

/**
 * @brief Kicks a client from the channel.
 *
 * This function removes the specified client from the list of invited clients,
 * removes the client from the list of channel operators, and removes the client
 * from the list of channel clients.
 *
 * @param client A pointer to the client to be kicked.
 */
void Channel::kick(Client* client)
{
	client->remove_channel_invited(this->get_name());
	remove_channel_operator(client);
	remove_channel_client(client);
}

/**
 * @brief Removes a client from the channel.
 *
 * This function removes the specified client from the channel by removing them
 * as a channel operator and removing them from the list of channel clients.
 *
 * @param client A pointer to the client to be removed from the channel.
 */
void Channel::part(Client* client)
{
	remove_channel_operator(client);
	remove_channel_client(client);
}

/**
 * @brief Broadcasts a message to all clients in the channel, excluding the
 * sender.
 *
 * This function iterates over the list of clients in the channel and calls the
 * `broadcast` function for each client, passing the sender, target, and message
 * as arguments. The `broadcast` function is called for both regular clients and
 * operator clients.
 *
 * @param sender A pointer to the client who sent the message.
 * @param target The target of the message.
 * @param message The message to be broadcasted.
 */
void Channel::broadcast(Client* sender, std::string target, std::string message)
{
	for (std::vector<Client*>::iterator it = this->_operator_clients.begin();
		 it != this->_operator_clients.end();
		 it++)
	{
		if (*it == sender)
			continue;
		(*it)->broadcast(sender, target, message);
		return;
	}
	for (std::vector<Client*>::iterator it = this->_clients.begin();
		 it != this->_clients.end();
		 it++)
	{
		if (*it == sender)
			continue;
		(*it)->broadcast(sender, target, message);
	}
}

/**
 * @brief Checks if a client with the given nickname is in the channel.
 *
 * @param nickname The nickname of the client to check.
 * @return true if the client is in the channel, false otherwise.
 */
bool Channel::is_client_in_channel(std::string nickname)
{
	for (std::vector<Client*>::iterator it = this->_clients.begin();
		 it != this->_clients.end();
		 ++it)
	{
		if ((*it)->get_nickname() == nickname)
			return true;
	}
	return false;
}

/**
 * @brief Checks if the channel is full.
 *
 * This function checks if the channel has reached its maximum capacity.
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
 * This function returns a boolean value indicating whether the channel is
 * invite-only or not.
 *
 * @return true if the channel is invite-only, false otherwise.
 */
bool Channel::is_channel_invite_only(void) const { return _invite_only; }
