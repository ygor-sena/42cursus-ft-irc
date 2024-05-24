/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:26:17 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/24 18:27:04 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel()
{
	topic = 0;
	_name = "";
	_key = "";
	_created_at = "";
	_has_key = false;
}

Channel::Channel(std::string name)
{
	topic = 0;
	_name = name;
	_key = "";
	_created_at = "";
	_has_key = false;
}

Channel::~Channel()
{
	this->_clients.clear();
	return ;
}

// GETTERS
std::string Channel::get_name(void) const
{
	return _name;
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

// Channel STATUS and checker functions
bool Channel::has_client(Client *client)
{
	for (std::vector<Client *>::const_iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if((*it)->get_nickname() == client->get_nickname())
			return true;
	}
	return false;
}

bool Channel::has_key(void)
{
	return this->_has_key;
}

bool Channel::is_channel_operator(std::string nickname)
{
	for (std::vector<Client *>::const_iterator it = this->_operator_clients.begin(); it != this->_operator_clients.end(); ++it)
	{
		if((*it)->get_nickname() == nickname)
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
