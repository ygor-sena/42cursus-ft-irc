/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:32:39 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/26 04:36:47 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#define PASS_CMD "PASS"

/*
 * Command: PASS
 * Parameters: <password>
 * Reference: https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.1
*/

/**
 * @brief Handles the PASSWORD command received from the client.
 *
 * This method processes the PASSWORD command received from the client and sends a response
 * to the client indicating that the client's password has been set.
 *
 * @param password The password to be assigned to the client.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_password(const std::string &buffer, const int fd)
{
	if (buffer.empty() || buffer == PASS_CMD) {
		_send_response(fd, ERR_NEEDMOREPARAMS(std::string("*")));
		return;
	}
	
	Client* client = _get_client(fd);
	
	if (client->get_is_authenticated()) {
		_send_response(fd, ERR_ALREADYREGISTERED(std::string("*")));
		return;
	}
	
	if (_password != buffer) {
		_send_response(fd, ERR_INCORPASS(std::string("*")));
	} else {
		client->set_is_authenticated(true);
	}

	// Registra o comando PASS recebido
    std::cout << "PASS command received from client " << buffer << std::endl;
}
