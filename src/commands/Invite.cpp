/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:31:16 by gilmar            #+#    #+#             */
/*   Updated: 2024/06/01 10:44:55 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp"

/*
 * Command: INVITE
 * Parameters: <nickname> <channel>
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.7
 * Example: INVITE Wiz #Twilight_Zone
 */

/**
 * Handles the "INVITE" command from a client.
 * This function is responsible for processing the invite command and sending appropriate responses to the client.
 *
 * @param buffer The command buffer received from the client.
 * @param fd The file descriptor of the client's socket connection.
 */
void Server::_handler_client_invite(const std::string& buffer, const int fd)
{
	Client* client = _get_client(fd);

	std::vector<std::string> params = _split_buffer(buffer, SPACE);
	if (params.size() < 2)
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
		std::string target_nickname = params[0];
		std::string target_channel = params[1];

		Client* invited_client = _get_client(target_nickname);
		Channel* channel = _get_channel(target_channel);

		if (!channel)
		{
			_send_response(fd, ERR_NOSUCHCHANNEL(target_channel));
			_reply_code = 403;
		}
		else if (!channel->has_client(client))
		{
			_send_response(fd, ERR_NOTONCHANNEL(target_channel));
			_reply_code = 442;
		}
		else if (!channel->is_channel_operator(client->get_nickname()))
		{
			_send_response(fd, ERR_NOPRIVILEGES(client->get_nickname()));
			_reply_code = 481;
		}
		else if (!invited_client)
		{
			_send_response(fd, ERR_NOSUCHNICK(target_channel, target_nickname));
			_reply_code = 401;
		}
		else if (channel->has_client(invited_client))
		{
			_send_response(fd,
						   ERR_USERONCHANNEL(target_nickname, target_channel));
			_reply_code = 443;
		}
		else
		{
			invited_client->add_channel_invited(target_channel);
			_send_response(fd,
						   RPL_INVITING(client->get_hostname(),
										target_channel,
										client->get_nickname(),
										target_nickname));
			_reply_code = 200;
		}
	}
}
