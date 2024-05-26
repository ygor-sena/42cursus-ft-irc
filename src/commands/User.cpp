/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:32:16 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/26 04:35:52 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#define USER_CMD "USER"

/*
 * Command: USER
 * Parameters: <username>
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.3
*/

/**
 * @brief Handles the USERNAME command received from the client.
 *
 * This method processes the USERNAME command received from the client and sends a response
 * to the client indicating that the client's username has been set.
 *
 * @param username The username to be assigned to the client.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_username(const std::string &buffer, const int fd)
{
	if (buffer.empty() || buffer.size() < 5 || buffer == USER_CMD) {
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
	
	if (!client->get_username().empty()) {
		_send_response(fd, ERR_ALREADYREGISTERED(std::string("*")));
		_reply_code = 462;
	} else {
		client->set_username(buffer);
		if (_client_is_ready_to_login(fd)) {
			client->set_is_logged(fd);
		}
		_reply_code = 200;
	}
	
	// Registra o comando USER recebido
    std::cout << "USER command received from client " << buffer << std::endl;
}
