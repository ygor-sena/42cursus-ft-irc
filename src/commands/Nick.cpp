/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:31:54 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/26 04:37:20 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#define NICK_CMD "NICK"

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
void Server::_handler_client_nickname(const std::string &buffer, const int fd)
{
	if (buffer.empty() || buffer.size() < 5 || buffer == NICK_CMD) {
		_send_response(fd, ERR_NEEDMOREPARAMS(std::string("*")));
		_reply_code = 461;
		return;
	}

	Client* client = _get_client(fd);
	
	if (!client->get_is_authenticated()) {
		_send_response(fd, ERR_NOTREGISTERED(std::string("*")));
		_reply_code = 451;
		return;
	}
	
	if (!_is_valid_nickname(buffer)) {
		_send_response(fd, ERR_ERRONEUSNICK(client->get_nickname()));
		_reply_code = 432;
		return;
	}
	
	if (_is_nickname_in_use(fd, buffer)) {
		_send_response(fd, ERR_NICKINUSE(client->get_nickname()));
		_reply_code = 433;
	} else {
		client->set_nickname(buffer);
		if (_client_is_ready_to_login(fd))
			client->set_is_logged(fd);
		_reply_code = 200;
	}

	// Registra o comando NICK recebido
    std::cout << "NICK command received from client " << buffer << std::endl;
}