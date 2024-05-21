/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:33:05 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/21 08:39:16 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: JOIN
 * Parameters: <channel>{,<channel>} [<key>{,<key>}]
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.1
*/

/**
 * @brief Handles the JOIN command received from the client.
 *
 * This method processes the JOIN command received from the client and sends a response
 * to the client indicating that the client has joined the channel.
 *
 * @param buffer The buffer containing the JOIN command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_join(const std::string &buffer, const int fd)
{
    Client* client = _get_client(fd);

    std::string channel = buffer;
    
    _send_response(fd, RPL_TOPICIS(client->get_nickname(), channel, std::string("Welcome to the channel")));
}