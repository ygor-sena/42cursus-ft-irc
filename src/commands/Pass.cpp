/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:32:39 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/25 21:06:37 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
void Server::_handler_client_password(const std::string &password, const int fd)
{
	Client* client = _get_client(fd);

	if (password.empty()) //-> Isso é o suficiente?
		_send_response(fd, ERR_NEEDMOREPARAMS(std::string("*")));
	else if (client->get_is_authenticated())
		_send_response(fd, ERR_ALREADYREGISTERED(std::string("*")));
	else if (_password != password)
		_send_response(fd, ERR_INCORPASS(std::string("*")));
	else
	{
		client->set_is_authenticated(true);
	}
}
