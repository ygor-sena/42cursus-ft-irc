/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:29:45 by gilmar            #+#    #+#             */
/*   Updated: 2024/06/01 09:02:49 by gilmar           ###   ########.fr       */
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
 * This method processes the QUIT command received from the client and sends a
 * response to the client indicating that the client has quit the server.
 *
 * To avoid memory leak, if the client being disconnected is the last one in a
 * channel, the channel is deleted.
 *
 * @param buffer The buffer containing the QUIT command parameters.
 * @param fd The file descriptor associated with the client that sent the
 * command.
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
