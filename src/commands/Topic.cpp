/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:30:59 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/26 04:36:18 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#define TOPIC_CMD "TOPIC"

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
	std::istringstream iss(buffer);
	std::string channel_name, topic;
	iss >> channel_name >> topic;

	Client* client = _get_client(fd);

	if (!client->get_is_logged()) {
		_send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
		return;
	}
	
	if (buffer == TOPIC_CMD) {
		_send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
		return;
	}

	Channel* channel = _get_channel(channel_name);

    if (!channel) {
        _send_response(fd, ERR_NOSUCHCHANNEL(channel_name));
        return;
    }

    if (!channel->is_client_in_channel(client->get_nickname())) {
        _send_response(fd, ERR_NOTONCHANNEL(client->get_nickname()));
        return;
    }

    if (topic.empty()) {
		if (channel->get_topic().empty()) {
        	_send_response(fd, RPL_NOTOPIC(client->get_nickname(), channel->get_name()));
		} else {
			_send_response(fd, RPL_TOPIC(client->get_nickname(), channel->get_name(), channel->get_topic()));
		}
    } else {
		if (!channel->is_channel_operator(client->get_nickname())) {
			_send_response(fd, ERR_NOTOPERATOR(channel->get_name()));
        } else {
			channel->set_topic(topic);
        	_send_response(fd, RPL_TOPIC(client->get_nickname(), channel_name, topic));
		}
	}

	// Registra o comando TOPIC recebido
    std::cout << "TOPIC command received from client " << buffer << std::endl;
}
