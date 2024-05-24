/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:31:16 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/24 18:25:36 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"

/*
 * Command: INVITE
 * Parameters: <nickname> <channel>
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.7
 * Example: INVITE Wiz #Twilight_Zone
*/

/**
 * @brief Handles the INVITE command received from the client.
 *
 * This method processes the INVITE command received from the client and sends a response
 * to the client indicating that the client has been invited to the channel.
 *
 * @param buffer The buffer containing the INVITE command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_invite(const std::string &buffer, const int fd)
{
	Client* client = _get_client(fd);

	// if client is not registered or authenticated, send ERR_NOTREGISTERED
	if (client->get_is_authenticated() && client->get_is_registered())
	{
		std::vector<std::string> params = _split_buffer(buffer, " ");
		if (params.size() < 2)
		{
			_send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
			_reply_code = 461;
			return;
		}

		std::string target_nickname = params[1];
		std::string target_channel = params[2];

		Channel *channel = this->_get_channel(target_channel);
		if (!channel)
		{
			_send_response(fd, ERR_NOSUCHCHANNEL(target_channel));
			_reply_code = 403;
			return;
		}

		// Verify if the client is in the channel
		if (channel->get_client_names().find(client->get_nickname()) == std::string::npos)
		{
			_send_response(fd, ERR_NOTONCHANNEL(target_channel));
			_reply_code = 442;
			return;
		}

		Client *invited_client = this->_get_client(target_nickname);
		if (!invited_client)
		{
			_send_response(fd, ERR_NOSUCHNICK(target_channel, target_nickname));
			_reply_code = 401;
			return;
		}

		// Verify if the invited_client is already in the channel
		if (channel->has_client(invited_client))
		{
			_send_response(fd, ERR_USERONCHANNEL(target_nickname, target_channel));
			_reply_code = 443;
			return;
		}

		channel->invite(invited_client);
		_send_response(fd, RPL_INVITING(invited_client->get_nickname(), "", channel, ""));
		_reply_code = 200;

		}
		else
		{
			_send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
			_reply_code = 451;
		}
		//_send_response(fd, RPL_INVITING(client->get_hostname(), "ft_transcendence", "Gilmar", "Ygor"));
		std::cout << "INVITE command received from client " << buffer << std::endl;
		return ;
}
