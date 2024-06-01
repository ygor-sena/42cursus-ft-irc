/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:29:45 by gilmar            #+#    #+#             */
/*   Updated: 2024/06/01 10:48:18 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: QUIT
 * Parameters: [<Quit message>]
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.6
 */

/**
 * Handles the "QUIT" command from a client.
 *
 * This function is responsible for processing the "QUIT" command received from a client.
 * It removes the client from all channels they are currently in and sends a response to
 * the client indicating that they have been disconnected. After handling the "QUIT" command,
 * the client is cleared from the server's client list.
 *
 * @param buffer The buffer containing the command and its arguments (not used in this function).
 * @param fd The file descriptor of the client's socket connection.
 */
void Server::_handler_client_quit(const std::string& /* buffer */, const int fd)
{
	Client* client = _get_client(fd);

	for (std::vector<Channel*>::iterator it = _channels.begin();
		 it != _channels.end();
		 ++it)
	{
		Channel* channel = *it;
		if (channel->has_client(client))
		{
			channel->quit(client);
			if (channel->get_channel_clients().size() == 0)
				delete channel;
		}
	}

	_send_response(fd, RPL_QUITMESSAGE(client->get_nickname()));
	_reply_code = 301;

	std::cout << RED << "Client <" << fd << "> Disconnected" << WHI
			  << std::endl;
	_clear_client(fd);
}
