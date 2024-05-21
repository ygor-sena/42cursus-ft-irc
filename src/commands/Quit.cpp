/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:29:45 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/21 08:49:57 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: QUIT
 * Parameters: [<Quit message>]
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.6
*/

/**
 * @brief Handles the QUIT command received from the client.
 *
 * This method processes the QUIT command received from the client and sends a response
 * to the client indicating that the client has quit the server.
 *
 * @param buffer The buffer containing the QUIT command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_quit(const std::string &buffer, const int fd)
{
    (void)buffer;
    _send_response(fd, RPL_QUITMESSAGE(_get_client(fd)->get_nickname()));
    _clear_client(fd);
}