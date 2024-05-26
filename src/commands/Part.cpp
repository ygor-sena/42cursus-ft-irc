/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:30:20 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/25 21:06:24 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: PART
 * Parameters: <channel>{,<channel>}
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.2
*/

/**
 * @brief Handles the PART command received from the client.
 *
 * This method processes the PART command received from the client and sends a response
 * to the client indicating that the client has left the channel.
 *
 * @param buffer The buffer containing the PART command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_part(const std::string &buffer, const int fd)
{
	Client* client = _get_client(fd);

	if (client->get_is_logged())
	{
		std::vector<std::string> params = _split_buffer(buffer, " ");
		if (params.size() < 1)
		{
			_send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
			_reply_code = 461;
			return ;
		}

		std::string channel_name = params[0];
		Channel *channel = this->_get_channel(channel_name);

		// Check if the channel exists
		if (!channel)
		{
			_send_response(fd, ERR_NOSUCHCHANNEL(channel_name));
			_reply_code = 403;
			return ;
		}

		// Verify if the client is in the channel
		if (channel->get_client_names().find(client->get_nickname()) == std::string::npos)
		{
			_send_response(fd, ERR_NOTONCHANNEL(channel_name));
			_reply_code = 442;
			return ;
		}

		// Remove the client from the channel
		channel->part(client);

		// Send a response to the client
		_send_response(fd, RPL_PART(client->get_hostname(), channel_name, client->get_nickname()));
		_reply_code = 200;
	}
	else
	{
		_send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
		_reply_code = 451;
	}
	//_send_response(fd, RPL_PART(client->get_hostname(), "ft_transcendence", client->get_nickname()));
}
