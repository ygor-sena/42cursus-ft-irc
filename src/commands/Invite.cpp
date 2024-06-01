/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:31:16 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/31 23:01:17 by gilmar           ###   ########.fr       */
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
 * @brief Handles the INVITE command received from the client.
 *
 * This method processes the INVITE command received from the client and sends a
 * response to the client indicating that the client has been invited to the
 * channel.
 *
 * @param buffer The buffer containing the INVITE command parameters.
 * @param fd The file descriptor associated with the client that sent the
 * command.
*/
void Server::_handler_client_invite(const std::string& buffer, const int fd)
{
	Client* client = _get_client(fd);

	if (!client->get_is_logged())
	{
		_send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
		_reply_code = 451;
		return;
	}

	std::vector<std::string> params = _split_buffer(buffer, " ");
	if (params.size() < 2)
	{
		_send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
		_reply_code = 461;
		return;
	}

	std::string target_nickname = params[0];
	std::string target_channel = params[1];

	Channel* channel = _get_channel(target_channel);
	if (!channel)
	{
		_send_response(fd, ERR_NOSUCHCHANNEL(target_channel));
		_reply_code = 403;
		return;
	}

	if (!channel->has_client(client))
	{
		_send_response(fd, ERR_NOTONCHANNEL(target_channel));
		_reply_code = 442;
		return;
	}

	if (!channel->is_channel_operator(client->get_nickname()))
	{
		_send_response(fd, ERR_NOPRIVILEGES(client->get_nickname()));
		_reply_code = 481;
		return;
	}

	Client* invited_client = _get_client(target_nickname);
	if (!invited_client)
	{
		_send_response(fd, ERR_NOSUCHNICK(target_channel, target_nickname));
		_reply_code = 401;
		return;
	}

	if (channel->has_client(invited_client))
	{
		_send_response(fd, ERR_USERONCHANNEL(target_nickname, target_channel));
		_reply_code = 443;
		return;
	}

	invited_client->add_channel_invited(target_channel);

	_send_response(fd,
				   RPL_INVITING(client->get_hostname(),
								target_channel,
								client->get_nickname(),
								target_nickname));
	_reply_code = 200;
}
