/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:32:39 by gilmar            #+#    #+#             */
/*   Updated: 2024/06/01 09:50:11 by caalbert         ###   ########.fr       */
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
 * This method processes the PASSWORD command received from the client and sends
 * a response to the client indicating that the client's password has been set.
 *
 * @param password The password to be assigned to the client.
 * @param fd The file descriptor associated with the client that sent the
 * command.
 */
/**
 * Handles the client password command.
 * This function is responsible for authenticating the client based on the provided password.
 * If the password is empty, it sends an error response and sets the reply code to 461.
 * If the client is already authenticated, it sends an error response and sets the reply code to 462.
 * If the provided password does not match the server's password, it sends an error response and sets the reply code to 464.
 * If the password is correct, it sets the client's authentication status to true and sets the reply code to 200.
 *
 * @param buffer The password command buffer.
 * @param fd The file descriptor of the client.
 */
void Server::_handler_client_password(const std::string& buffer, const int fd)
{
	Client* client = _get_client(fd);

	if (buffer.empty())
	{
		_send_response(fd, ERR_NEEDMOREPARAMS(std::string("*")));
		_reply_code = 461;
	}
	else if (client->get_is_authenticated())
	{
		_send_response(fd, ERR_ALREADYREGISTERED(std::string("*")));
		_reply_code = 462;
	}
	else if (_password != buffer)
	{
		_send_response(fd, ERR_INCORPASS(std::string("*")));
		_reply_code = 464;
	}
	else
	{
		client->set_is_authenticated(true);
		_reply_code = 200;
	}
}
