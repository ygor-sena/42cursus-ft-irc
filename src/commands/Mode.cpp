/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:30:34 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/21 08:48:10 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: MODE
 * Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>]
 * Reference: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.3
*/

/**
 * @brief Handles the MODE command received from the client.
 *
 * This method processes the MODE command received from the client and sends a response
 * to the client indicating the channel modes supported by the server.
 *
 * @param buffer The buffer containing the MODE command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_mode(const std::string &buffer, const int fd)
{
    (void)fd;
    (void)buffer;
    Client* client = _get_client(fd);

    _send_response(fd, RPL_CHANNELMODES(client->get_nickname(), "ft_trancendence", "+nt"));
}