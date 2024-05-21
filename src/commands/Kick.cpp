/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:30:47 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/21 08:43:50 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: Kick 
 * Parameters: <channel> <user> [<comment>]
 * Reference: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.8
*/

/**
 * @brief Handles the KICK command received from the client.
 *
 * This method processes the KICK command received from the client and sends a response
 * to the client indicating that the client has been kicked from the channel.
 *
 * @param buffer The buffer containing the KICK command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_kick(const std::string &buffer, const int fd)
{
    (void)fd;
    (void)buffer;
    Client* client = _get_client(fd);

    _send_response(fd, RPL_KICK(client->get_nickname(), "ft_trancendence", "gilmar", "Ygor", "You have been kicked"));
}