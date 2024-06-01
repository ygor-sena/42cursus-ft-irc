/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:32:16 by gilmar            #+#    #+#             */
/*   Updated: 2024/06/01 09:51:44 by caalbert         ###   ########.fr       */
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
/**
 * Handles the client's username command.
 *
 * This function is responsible for processing the client's username command and performing the necessary actions based on the command's parameters and the client's state.
 *
 * @param buffer The command buffer containing the client's username.
 * @param fd The file descriptor of the client's connection.
 */
void Server::_handler_client_username(const std::string& buffer, const int fd)
{
	Client* client = _get_client(fd);

	if (buffer.empty())
	{
		_send_response(fd, ERR_NEEDMOREPARAMS(std::string("*")));
		_reply_code = 461;
	}
	else if (!client || !client->get_is_authenticated())
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
			_send_response(fd,
						   RPL_CONNECTED(_get_hostname(),
										 client->get_nickname(),
										 client->get_hostname()));
			_reply_code = 001;
		}
		else
		{
			_reply_code = 200;
		}
	}
}
