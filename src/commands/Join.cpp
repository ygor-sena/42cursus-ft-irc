/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:33:05 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/24 18:25:42 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: JOIN
 * Parameters: <channel>{,<channel>} [<key>{,<key>}]
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.1
*/

/**
 * @brief Handles the JOIN command received from the client.
 *
 * This method processes the JOIN command received from the client and sends a response
 * to the client indicating that the client has joined the channel.
 *
 * @param buffer The buffer containing the JOIN command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_join(const std::string &buffer, const int fd)
{
	Client* client = _get_client(fd);

	if (client->get_is_authenticated() && client->get_is_registered())
	{
		std::vector<std::string> params = _split_buffer(buffer, " ");
		if (params.size() < 1)
		{
			_send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
			return ;
		}
		
		std::string joining_channel = params[0];
		if (joining_channel[0] != '#')
		{
			_send_response(fd, ERR_NOSUCHCHANNEL(joining_channel));
			return ;
		}

		Channel *channel = this->_get_channel(joining_channel);
		if (!channel)
		{
			channel = new Channel(joining_channel);
			this->_add_channel(channel);
		}
		else
		{
			if (channel->has_client(client))
			{
				_send_response(fd, ERR_ALREADYREGISTERED(client->get_nickname()));
				return ;
			}
		}

		channel->join(client);
		
		// TODO: Topics are not implemented yet
		// TODO: Channel keys are not implemented yet

	}
	else
	{
		_send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
		return ;
	}
	
	std::string channel = buffer;
	
	_send_response(fd, RPL_TOPICIS(client->get_nickname(), channel, std::string("Welcome to the channel")));
}