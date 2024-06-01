/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:32:16 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/31 21:53:04 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: USER
 * Parameters: <username>
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.3
*/

/**
 * @brief Handles the USERNAME command received from the client.
 *
 * This method processes the USERNAME command received from the client and sends
 * a response to the client indicating that the client's username has been set.
 *
 * @param username The username to be assigned to the client.
 * @param fd The file descriptor associated with the client that sent the
 * command.
 */
void Server::_handler_client_username(const std::string& buffer, const int fd)
{
	Client* client = _get_client(fd);

	if (buffer.empty())
	{
		_send_response(fd, ERR_NEEDMOREPARAMS(std::string("*")));
		_reply_code = 461;
	}
	else if (!client)
	{
		_send_response(fd, ERR_NOTREGISTERED(std::string("*")));
		_reply_code = 451;
	}
	else if (!client->get_is_authenticated())
	{
		_send_response(fd, ERR_NOTREGISTERED(std::string("*")));
		_reply_code = 451;
	}
	else if (!client->get_already_registered())
	{
		_send_response(fd, ERR_ALREADYREGISTERED(client->get_nickname()));
		_reply_code = 462;
	}
	else
	{
		client->set_username(buffer);
		if (_client_is_ready_to_login(fd))
		{
			client->set_is_logged(fd);
			_send_response(fd, RPL_CONNECTED(_get_hostname(), client->get_nickname(), client->get_hostname()));
			_reply_code = 001;
		}
		else
		{
			_reply_code = 200;
		}
	}
}
