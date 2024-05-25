/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:31:33 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/24 18:22:35 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: PRIVMSG
 * Parameters: <receiver>{,<receiver>} <text to be sent>
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.4.1
*/

#include <vector>
#include <string>
#include <sstream>
#include <utility>

/* std::pair<std::vector<std::string>, std::string> splitMessage(const std::string& message) {
    std::istringstream iss(message);
    std::string receiversStr, text;

    // Get the receivers string and the text
    std::getline(iss, receiversStr, ' ');
    std::getline(iss, text);

    // Split the receivers string into individual receivers
    std::vector<std::string> receivers;
    std::istringstream receiversStream(receiversStr);
    std::string receiver;
    while (std::getline(receiversStream, receiver, ',')) {
        receivers.push_back(receiver);
    }

    // Create a pair and return it
    std::pair<std::vector<std::string>, std::string> result;
    result.first = receivers;
    result.second = text;
    return result;
} */

/**
 * @brief Handles the PRIVMSG command received from the client.
 *
 * This method processes the PRIVMSG command received from the client and sends a response
 * to the client indicating that the message has been sent.
 *
 * @param buffer The buffer containing the PRIVMSG command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_privmsg(const std::string &buffer, const int fd)
{
    (void)fd;
    (void)buffer;
    Client* client = _get_client(fd);

/*
    std::vector<std::string> params = _split_buffer(buffer, " ");

    if (client->get_is_registered())
    {
        if (params.size() < 2)
        {
            _send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname(), "PRIVMSG"));
            return;
        }
    }
    
    std::pair<std::vector<std::string>, std::string> privmsg = splitMessage(params[1]);
    std::vector<std::string> receivers = privmsg.first;
    std::string text = privmsg.second;

    if (receivers.size() == 0 && privmsg.second.empty())
    {
        _send_response(fd, ERR_NOSUCHNICK(client->get_nickname()));
        return;
    }
     */

    _send_response(fd, RPL_PRIVMSG(client->get_hostname(), "ft_transcendence", "Hello, Carlos!"));
}