/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:30:20 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/21 08:49:00 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: PART
 * Parameters: <channel>{,<channel>}
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.2
*/

/**
 * @brief Handles the PART command received from the client.
 *
 * This method processes the PART command received from the client and sends a response
 * to the client indicating that the client has left the channel.
 *
 * @param buffer The buffer containing the PART command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_part(const std::string &buffer, const int fd)
{
    (void)fd;
    (void)buffer;
    Client* client = _get_client(fd);

    _send_response(fd, RPL_PART(client->get_hostname(), "ft_trancendence", client->get_nickname()));
}
