/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:33:05 by gilmar            #+#    #+#             */
/*   Updated: 2024/06/01 10:45:23 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: JOIN
 * Parameters: <channel>{,<channel>} [<key>{,<key>}]
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.1
 */

/**
 * Handles the JOIN command from a client.
 *
 * This function is responsible for processing the JOIN command received from a client.
 * It checks the validity of the command and performs the necessary actions, such as
 * creating a new channel, joining an existing channel, or sending error responses.
 *
 * @param buffer The command buffer received from the client.
 * @param fd The file descriptor of the client's socket.
 */
void Server::_handler_client_join(const std::string& buffer, const int fd)
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
		std::string joining_channel = params[0];
		std::string channel_key = (params.size() > 1) ? params[1] : "";

		Channel* channel = _get_channel(joining_channel);

		if (joining_channel[0] != '#')
		{
			_send_response(
				fd,
				ERR_BADCHANMASK(
					_get_hostname(), client->get_nickname(), joining_channel));
			_reply_code = 403;
		}
		else if (!channel)
		{
			channel = new Channel(joining_channel);
			_add_channel(channel);
			channel->join(client);
			channel->set_channel_operator(client);
			_send_response(fd,
						   RPL_JOINMSG(client->get_nickname(),
									   client->get_hostname(),
									   joining_channel));
			_reply_code = 200;
		}
		else if (channel->has_client(client))
		{
			_send_response(fd, ERR_ALREADYREGISTERED(client->get_nickname()));
			_reply_code = 462;
		}
		else if (channel->is_channel_full())
		{
			_send_response(
				fd, ERR_CHANNELISFULL(client->get_nickname(), joining_channel));
			_reply_code = 422;
		}
		else if (channel->is_channel_invite_only() &&
				 !client->is_channel_invited(joining_channel))
		{
			_send_response(
				fd,
				ERR_INVITEONLYCHAN(client->get_nickname(), joining_channel));
			_reply_code = 473;
		}
		else if (channel->has_key() &&
				 channel_key != channel->get_channel_key())
		{
			_send_response(
				fd, ERR_BADCHANNELKEY(client->get_nickname(), joining_channel));
			_reply_code = 475;
		}
		else
		{
			_send_response(fd,
						   RPL_JOINMSG(client->get_nickname(),
									   client->get_hostname(),
									   joining_channel));
			channel->join(client);
			_reply_code = 200;
		}
	}
}
