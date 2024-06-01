/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:31:33 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/31 23:11:18 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

/*
 * Command: PRIVMSG
 * Parameters: <receiver>{,<receiver>} <text to be sent>
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.4.1
 */

/**
 * @brief Splits a string into tokens based on a delimiter.
 *
 * This function takes a string and a delimiter as input and splits the string
 * into tokens based on the delimiter. The tokens are stored in a vector and
 * returned.
 *
 * @param s The string to be split.
 * @param delimiter The delimiter used to split the string.
 * @return A vector of strings containing the tokens.
 */
std::vector<std::string> split_parameters(const std::string& s,
										  const std::string& delimiter)
{
	std::vector<std::string> tokens;
	size_t start = 0;
	size_t end = s.find(delimiter);
	while (end != std::string::npos)
	{
		tokens.push_back(s.substr(start, end - start));
		start = end + delimiter.length();
		end = s.find(delimiter, start);
	}
	tokens.push_back(s.substr(start, end));
	return tokens;
}

/**
 * @brief Handles the PRIVMSG command received from the client.
 *
 * This method processes the PRIVMSG command received from the client and sends
 * a response to the client indicating that the message has been sent.
 *
 * @param buffer The buffer containing the PRIVMSG command parameters.
 * @param fd The file descriptor associated with the client that sent the
 * command.
 */
void Server::_handler_client_privmsg(const std::string& buffer, const int fd)
{
	Client* client = _get_client(fd);

	std::vector<std::string> params = _split_buffer(buffer, " ");

	if (client->get_is_logged())
	{
		if (params.size() < 2)
		{
			_send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
			_reply_code = 461;
			return;
		}

		std::vector<std::string> receivers = split_parameters(params[0], ",");

		for (std::vector<std::string>::iterator it = receivers.begin();
			 it != receivers.end();
			 ++it)
		{
			if ((*it)[0] == '#')
			{
				Channel* target_channel = this->_get_channel(*it);
				if (!target_channel)
				{
					_send_response(fd, ERR_NOSUCHCHANNEL(*it));
					_reply_code = 403;
					return;
				}

				if (!target_channel->has_client(client))
				{
					_send_response(fd,
								   ERR_NOTONCHANNEL(client->get_nickname()));
					_reply_code = 442;
					return;
				}
			}
			else
			{
				Client* target_client = this->_get_client(*it);
				if (!target_client)
				{
					_send_response(fd,
								   ERR_NOSUCHNICK(std::string(""),
												  client->get_nickname()));
					_reply_code = 401;
					return;
				}
			}
		}

		for (std::vector<std::string>::iterator it = receivers.begin();
			 it != receivers.end();
			 ++it)
		{
			if ((*it)[0] == '#')
			{
				Channel* target_channel = this->_get_channel(*it);
				target_channel->broadcast(
					client, target_channel->get_name(), params[1]);
			}
			else
			{
				Client* target_client = this->_get_client(*it);
				_send_response(target_client->get_fd(),
							   RPL_PRIVMSG(client->get_nickname(),
										   client->get_hostname(),
										   target_client->get_nickname(),
										   params[1]));
			}
		}
	}
	else
	{
		_send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
		_reply_code = 451;
	}
}