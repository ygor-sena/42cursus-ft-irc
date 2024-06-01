/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:30:34 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/31 23:00:49 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
/*
 * Command: MODE
 * Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>]
 * Reference: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.3
 */

/**
 * @brief Handles the MODE command received from the client.
 *
 * This method processes the MODE command received from the client and sends a
 * response to the client indicating the channel modes supported by the server.
 *
 * @param buffer The buffer containing the MODE command parameters.
 * @param fd The file descriptor associated with the client that sent the
 * command.
 */
void Server::_handler_client_mode(const std::string& buffer, const int fd)
{
	std::istringstream iss(buffer);
	std::string channelName, modeFlags, argument;

	iss >> channelName >> modeFlags;
	if (iss >> argument)
	{
		iss >> argument;
	}

	Client* client = _get_client(fd);
	Channel* channel = _get_channel(channelName);

	if (channelName.empty() || modeFlags.empty())
	{
		_send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
		_reply_code = 461;
	}
	else if (!channel)
	{
		_send_response(fd, ERR_NOSUCHCHANNEL(channelName));
		_reply_code = 403;
	}
	else if (!channel->is_channel_operator(client->get_nickname()))
	{
		_send_response(fd, ERR_CHANOPRIVSNEEDED(channelName));
		_reply_code = 482;
	}
	else if (!_process_mode_flags(
				 modeFlags, channel, _get_client(argument), argument, fd))
	{
		return;
	}
	else
	{
		_reply_code = 200;
	}
}

bool Server::_process_mode_flags(const std::string& modeFlags, Channel* channel,
								 Client* targetClient,
								 const std::string& argument, const int fd)
{
	bool addMode = false;
	char mode = 0;

	for (size_t i = 0; i < modeFlags.size(); ++i)
	{
		char flag = modeFlags[i];
		if (flag == '+' || flag == '-')
		{
			addMode = (flag == '+');
		}
		else
		{
			mode = flag;
			if (!_apply_mode_flag(
					channel, targetClient, mode, addMode, argument))
			{
				_send_response(fd,
							   ERR_UNKNOWNMODE(_get_client(fd)->get_nickname(),
											   channel->get_name(),
											   mode));
				_reply_code = 472;
				return false;
			}
		}
	}

	if (targetClient)
	{
		std::string signal = addMode ? "+" : "-";
		_send_response(fd,
					   RPL_UMODEIS(targetClient->get_nickname(),
								   targetClient->get_hostname(),
								   channel->get_name(),
								   signal,
								   mode,
								   argument));
	}

	return true;
}

bool Server::_apply_mode_flag(Channel* channel, Client* targetClient, char mode,
							  bool addMode, const std::string& argument)
{
	switch (mode)
	{
	case 'i':
		_set_invite_only_mode(channel, addMode);
		break;
	case 't':
		_set_topic_restriction_mode(channel, addMode);
		break;
	case 'k':
		_set_channel_key_mode(channel, argument, addMode);
		break;
	case 'o':
		_set_channel_operator_mode(channel, targetClient, addMode);
		break;
	case 'l':
		_set_channel_limit_mode(channel, argument, addMode);
		break;
	default:
		return false;
	}
	return true;
}

void Server::_set_invite_only_mode(Channel* channel, bool addMode)
{
	if (addMode)
	{
		channel->set_invite_only();
	}
	else
	{
		channel->remove_invite_only();
	}
}

void Server::_set_topic_restriction_mode(Channel* channel, bool addMode)
{
	if (addMode)
	{
		channel->set_topic_restriction();
	}
	else
	{
		channel->remove_topic_restriction();
	}
}

void Server::_set_channel_key_mode(Channel* channel, const std::string& key,
								   bool addMode)
{
	if (addMode)
	{
		channel->set_key(key);
	}
	else
	{
		channel->remove_key();
	}
}

void Server::_set_channel_operator_mode(Channel* channel, Client* client,
										bool addMode)
{
	if (addMode && client)
	{
		channel->set_channel_operator(client);
	}
	else if (client)
	{
		channel->remove_channel_operator(client);
	}
}

void Server::_set_channel_limit_mode(Channel* channel,
									 const std::string& limitStr, bool addMode)
{
	if (addMode)
	{
		if (!limitStr.empty())
		{
			channel->set_limit(std::atoi(limitStr.c_str()));
		}
	}
	else
	{
		channel->remove_limit();
	}
}