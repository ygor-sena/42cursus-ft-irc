/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:31:54 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/24 18:25:54 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: NICK
 * Parameters: <nickname>
 * Reference: https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.2
*/

/**
 * @brief Handles the NICK command received from the client.
 *
 * This method processes the NICK command received from the client and sends a response
 * to the client indicating that the client's nickname has been changed.
 *
 * @param nickname The new nickname to be assigned to the client.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_nickname(const std::string &nickname, const int fd)
{
	Client* client = _get_client(fd);

	if (nickname.empty() || nickname.size() < 5)
	{
		_send_response(fd, ERR_NEEDMOREPARAMS(std::string("*")));
		_reply_code = 461;
	}
	else if (!client->get_is_registered())
	{
		_send_response(fd, ERR_NOTREGISTERED(std::string("*")));
		_reply_code = 451;
	}
	else if (!_is_valid_nickname(nickname))
	{
	   _send_response(fd, ERR_ERRONEUSNICK(client->get_nickname()));
		_reply_code = 432;
	}
	else if (_is_nickname_in_use(fd, nickname))
	{
		_send_response(fd, ERR_NICKINUSE(client->get_nickname()));
		_reply_code = 433;
	}
	else
	{
		client->set_nickname(nickname);
		if (_client_is_ready_to_login(fd))
			client->set_is_logged(fd);
		_reply_code = 200;
	}
}