/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:26:17 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/26 05:39:32 by gilmar           ###   ########.fr       */
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
	_password = "";
	_created_at = "";
	_has_key = false;
}

Channel::Channel(std::string name)
{
	_key = "";
	_topic = "";
	_name = name;
	_password = "";
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

std::string Channel::get_name(void) const
{
	return _name;
}

std::vector<Client *> Channel::get_operator_clients(void)
{
	return this->_operator_clients;
}

std::vector<Client *> Channel::get_channel_clients(void)
{
	return this->_clients;
}

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

int Channel::get_clients_size(void) const
{
	return this->_clients.size();
}

std::string Channel::get_topic(void) const
{
	return _topic;
}

std::string Channel::get_channel_password(void) const
{
	return _password;
}

void Channel::set_channel_operator(Client *client)
{
	client->set_is_operator(true);
	this->_operator_clients.push_back(client);
	return ;
}

void Channel::set_topic(std::string topic)
{
	_topic = topic;
}

void Channel::set_limit(int limit)
{
	_limit = limit;
}

void Channel::set_invite_only()
{
	_invite_only = true;
}

void Channel::set_topic_restriction()
{
	_topic_restriction = true;
}

void Channel::set_password(std::string password)
{
	_password = password;
}

void Channel::remove_limit(void)
{
	_limit = -1;
}

void Channel::remove_password(void)
{
	_password = "";
}

void Channel::remove_topic_restriction(void)
{
	_topic_restriction = false;
}

void Channel::remove_invite_only(void)
{
	_invite_only = false;
}

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

/*
** --------------------------------- OTHERS ---------------------------------
*/

// Channel STATUS and checker functions
bool Channel::has_client(Client *client)
{
	for (std::vector<Client *>::const_iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if ((*it)->get_nickname() == client->get_nickname())
			return true;
	}
	return false;
}

bool Channel::has_key(void) const
{
	return this->_has_key;
}

bool Channel::is_channel_operator(std::string nickname)
{
	for (std::vector<Client *>::iterator it = _operator_clients.begin(); it != this->_operator_clients.end(); ++it)
	{
		if ((*it)->get_nickname() == nickname)
			return true;
	}
	return false;
}

// Member functions for CHANNEL MANAGEMENT
void Channel::invite(Client *client)
{
	_clients.push_back(client);
	return ;
}

void Channel::join(Client *client)
{
	_clients.push_back(client);
	return ;
}

void Channel::kick(Client *client)
{
	for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if((*it)->get_nickname() == client->get_nickname())
		{
			this->_clients.erase(it);
			return ;
		}
	}
	return ;
}

void Channel::part(Client *client)
{
	// Remove client from channel operators
	for (std::vector<Client *>::iterator it_op = this->_operator_clients.begin(); it_op != this->_operator_clients.end(); ++it_op)
	{
		if((*it_op)->get_nickname() == client->get_nickname())
		{
			this->_operator_clients.erase(it_op);
			return ;
		}
	}

	for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if((*it)->get_nickname() == client->get_nickname())
		{
			std::cout << (*it)->get_nickname() << std::endl;
			this->_clients.erase(it);
			return ;
		}
	}
}

void Channel::broadcast(Client *sender, std::string command, std::string target, std::string message)
{
	for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++) {
		if (*it == sender)
			continue ;
		std::cout << "2) " << this->get_name() << " : " << (*it)->get_nickname() << std::endl;
		(*it)->broadcast(sender, command, target, message);
	}
	return ;
}

bool Channel::is_client_in_channel(std::string nickname)
{
	for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if((*it)->get_nickname() == nickname)
			return true;
	}
	return false;
}

bool Channel::is_channel_full(void) const
{
	if (_limit == -1)
		return false;
	if (_clients.size() >= _limit)
		return true;
	return false;
}

bool Channel::is_channel_invite_only(void) const
{
	return _invite_only;
}

bool Channel::has_password(void) const
{
	return _password != "";
}