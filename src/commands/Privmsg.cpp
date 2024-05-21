/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:31:33 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/21 08:51:42 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: PRIVMSG
 * Parameters: <receiver>{,<receiver>} <text to be sent>
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.4.1
*/

/**
 * @brief Handles the PRIVMSG command received from the client.
 *
 * This method processes the PRIVMSG command received from the client and sends a response
 * to the client indicating that the message has been sent.
 *
 * @param buffer The buffer containing the PRIVMSG command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_privmsg(const std::string &buffer, const int fd)
{
    (void)fd;
    (void)buffer;
    Client* client = _get_client(fd);

    _send_response(fd, RPL_PRIVMSG(client->get_hostname(), "ft_trancendence", "Hello, Carlos!"));
}