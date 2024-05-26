/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:30:47 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/25 21:05:57 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: Kick 
 * Parameters: <channel> <user> [<comment>]
 * Reference: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.8
*/

/**
 * @brief Handles the KICK command received from the client.
 *
 * This method processes the KICK command received from the client and sends a response
 * to the client indicating that the client has been kicked from the channel.
 *
 * @param buffer The buffer containing the KICK command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_kick(const std::string &buffer, const int fd)
{
	Client* client = _get_client(fd);

/* 	std::cout << client->get_nickname() << " is trying to kick a user" << std::endl;
	std::cout << client->get_fd() << std::endl;
	std::cout << client->get_password() << std::endl;
	std::cout << client->get_is_authenticated() << std::endl;
	std::cout << client->get_is_registered() << std::endl;
	std::cout << client->get_is_operator() << std::endl; */

	if (client->get_is_logged())
	{
		std::vector<std::string> params = _split_buffer(buffer, " ");
		std::vector<std::string> comments = _split_buffer(params[1], " ");

		// Check if the command has the minimum number of parameters
		if (params.size() < 2)
		{
			_send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
			_reply_code = 461;
			return;
		}

		std::string channel_name = params[0];

		// Check if the channel exists
		Channel *channel = this->_get_channel(channel_name);
		if (!channel)
		{
			_send_response(fd, ERR_NOSUCHCHANNEL(channel_name));
			_reply_code = 403;
			return;
		}

		// Check if the client is in the channel
		if (!channel->has_client(client))
		{
			_send_response(fd, ERR_NOTONCHANNEL(channel_name));
			_reply_code = 442;
			return;
		}

		// Check if the client is a channel operator
		if (channel->is_channel_operator(client->get_nickname()) == false)
		{
			_send_response(fd, ERR_CHANOPRIVSNEEDED(client->get_nickname(), channel_name));
			_reply_code = 482;
			return;
		}

		Client *target_client = this->_get_client(comments[0]);

		// Check if the target client exists
		if (!target_client)
		{
			_send_response(fd, ERR_NOSUCHNICK(channel_name, client->get_nickname()));
			_reply_code = 401;
			return;
		}

		// Check if the target client is in the channel
		if (!channel->has_client(target_client))
		{
			_send_response(fd, ERR_USERNOTINCHANNEL(target_client->get_nickname(), channel_name));
			_reply_code = 441;
			return;
		}

		// Check if option parameter <comment> exists
		if (params.size() > 2)
		{
			_send_response(fd, RPL_KICK(client->get_hostname(), channel_name, client->get_nickname(), target_client->get_nickname(), comments[1]));
		}
		else
		{
			_send_response(fd, RPL_KICK(client->get_hostname(), channel_name, client->get_nickname(), target_client->get_nickname(), ""));
		}
		channel->kick(target_client);
		_reply_code = 200;
	}
	else
	{
		_send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
		_reply_code = 451;
		return ;
	}
	//_send_response(fd, RPL_KICK(client->get_nickname(), "ft_transcendence", "gilmar", "Ygor", "You have been kicked"));
}