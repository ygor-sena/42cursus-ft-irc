/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:31:54 by gilmar            #+#    #+#             */
/*   Updated: 2024/06/01 09:46:30 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Replies.hpp"
#include "Server.hpp"

/*
 * Command: NICK
 * Parameters: <nickname>
 * Reference: https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.2
 */

/**
 * @brief Handles the NICK command received from the client.
 *
 * This method processes the NICK command received from the client and sends a
 * response to the client indicating that the client's nickname has been
 * changed.
 *
 * @param nickname The new nickname to be assigned to the client.
 * @param fd The file descriptor associated with the client that sent the
 * command.
 */
/**
 * Handles the client nickname command.
 * This function is responsible for processing the client's nickname command and performing the necessary actions based on the command's parameters and the client's state.
 *
 * @param buffer The command buffer containing the client's nickname command.
 * @param fd The file descriptor of the client's connection.
 */
void Server::_handler_client_nickname(const std::string& buffer, const int fd)
{
	Client* client = _get_client(fd);

	if (buffer.empty())
	{
		_send_response(fd, ERR_NEEDMOREPARAMS(std::string("*")));
		_reply_code = 461;
	}
	else if (!client->get_is_authenticated())
	{
		_send_response(fd, ERR_NOTREGISTERED(std::string("*")));
		_reply_code = 451;
	}
	else if (!_is_valid_nickname(buffer))
	{
		_send_response(fd, ERR_ERRONEUSNICK(client->get_nickname()));
		_reply_code = 432;
	}
	else if (_is_nickname_in_use(fd, buffer))
	{
		_send_response(fd, ERR_NICKINUSE(client->get_nickname()));
		_reply_code = 433;
	}
	else
	{
		client->set_nickname(buffer);
		if (_client_is_ready_to_login(fd))
		{
			client->set_is_logged(fd);
			_send_response(fd,
						   RPL_CONNECTED(_get_hostname(),
										 client->get_nickname(),
										 client->get_hostname()));
			_reply_code = 001;
			return;
		}
		_reply_code = 200;
	}
}
