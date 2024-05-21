/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:32:16 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/21 08:50:38 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
void Server::_handler_client_username(const std::string &username, const int fd)
{
    Client* client = _get_client(fd);

    if (username.empty() || username.size() < 5)
        _send_response(fd, ERR_NOTENOUGHPARAM(std::string("*")));
    else if (!client->get_is_registered())
        _send_response(fd, ERR_NOTREGISTERED(std::string("*")));
    else if (!client->get_username().empty())
        _send_response(fd, ERR_ALREADYREGISTERED(std::string("*")));
    else
    {
        client->set_username(username);
        if (_client_is_ready_to_login(fd))
            client->set_is_logged(fd);
    }
}