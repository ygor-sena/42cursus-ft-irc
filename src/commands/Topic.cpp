/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:30:59 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/24 18:26:37 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: TOPIC
 * Parameters: <channel> [<topic>]
 * Reference: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.4
*/

/**
 * @brief Handles the TOPIC command received from the client.
 *
 * This method processes the TOPIC command received from the client and sends a response
 * to the client indicating the topic of the channel.
 *
 * @param buffer The buffer containing the TOPIC command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_topic(const std::string &buffer, const int fd)
{
	(void)fd;
	(void)buffer;
	Client* client = _get_client(fd);

	_send_response(fd, RPL_TOPICIS(client->get_nickname(), "ft_transcendence", "Welcome to the channel"));
}