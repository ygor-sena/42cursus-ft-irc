/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:30:20 by gilmar            #+#    #+#             */
/*   Updated: 2024/06/01 09:47:04 by caalbert         ###   ########.fr       */
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
 * This method processes the PART command received from the client and sends a
 * response to the client indicating that the client has left the channel.
 *
 * @param buffer The buffer containing the PART command parameters.
 * @param fd The file descriptor associated with the client that sent the
 * command.
 */
/**
 * Handles the PART command from a client.
 *
 * This function is responsible for processing the PART command received from a client.
 * The PART command is used to instruct the server that the client wishes to leave a specific channel.
 *
 * @param buffer The command buffer containing the PART command and its parameters.
 * @param fd The file descriptor of the client's socket connection.
 */
void Server::_handler_client_part(const std::string& buffer, const int fd)
{
	Client* client = _get_client(fd);

	std::vector<std::string> params = _split_buffer(buffer, SPACE);

	if (buffer.empty())
	{
		_send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
		_reply_code = 461;
	}
	else if (!client->get_is_logged())
	{
		_send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
		_reply_code = 451;
	}
	else
	{
		std::string channel_name = params[0];

		Channel* channel = _get_channel(channel_name);
		if (!channel)
		{
			_send_response(fd, ERR_NOSUCHCHANNEL(channel_name));
			_reply_code = 403;
		}
		else if (!channel->has_client(client))
		{
			_send_response(fd, ERR_NOTONCHANNEL(channel_name));
			_reply_code = 442;
		}
		else
		{
			channel->part(client);
			_send_response(fd,
						   RPL_PART(client->get_hostname(),
									channel_name,
									client->get_nickname()));
			_reply_code = 200;
		}
	}
}
