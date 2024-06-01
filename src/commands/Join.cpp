/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:33:05 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/31 22:59:15 by gilmar           ###   ########.fr       */
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
 * This method processes the JOIN command received from the client and sends a
 * response to the client indicating that the client has joined the channel.
 *
 * @param buffer The buffer containing the JOIN command parameters.
 * @param fd The file descriptor associated with the client that sent the
 * command.
 */
void Server::_handler_client_join(const std::string& buffer, const int fd)
{
	Client* client = _get_client(fd);

	if (!client->get_is_logged())
	{
		_send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
		_reply_code = 451;
		return;
	}

	std::vector<std::string> params = _split_buffer(buffer, SPACE);
	if (params.size() < 1)
	{
		_send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
		_reply_code = 461;
		return;
	}

	std::string joining_channel = params[0];
	std::string channel_key = (params.size() > 1) ? params[1] : "";

	if (joining_channel[0] != '#')
	{
		_send_response(fd, ERR_NOSUCHCHANNEL(joining_channel));
		_reply_code = 403;
		return;
	}

	Channel* channel = _get_channel(joining_channel);
	if (!channel)
	{
		channel = new Channel(joining_channel);
		_add_channel(channel);
		channel->join(client);
		channel->set_channel_operator(client);
		_send_response(fd,
					   RPL_JOINMSG(client->get_nickname(),
								   client->get_hostname(),
								   joining_channel));
		return;
	}

	if (channel->has_client(client))
	{
		_send_response(fd, ERR_ALREADYREGISTERED(client->get_nickname()));
		_reply_code = 462;
		return;
	}

	if (channel->is_channel_full())
	{
		_send_response(
			fd, ERR_CHANNELISFULL(client->get_nickname(), joining_channel));
		_reply_code = 422;
		return;
	}

	if (channel->is_channel_invite_only() &&
		!client->is_channel_invited(joining_channel))
	{
		_send_response(
			fd, ERR_INVITEONLYCHAN(client->get_nickname(), joining_channel));
		_reply_code = 473;
		return;
	}

	if (channel->has_key() && channel_key != channel->get_channel_key())
	{
		_send_response(
			fd, ERR_BADCHANNELKEY(client->get_nickname(), joining_channel));
		_reply_code = 475;
		return;
	}

	_send_response(
		fd,
		RPL_JOINMSG(
			client->get_nickname(), client->get_hostname(), joining_channel));

	channel->join(client);
	_reply_code = 200;
}