/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:31:33 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/27 10:28:22 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"

/*
 * Command: PRIVMSG
 * Parameters: <receiver>{,<receiver>} <text to be sent>
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.4.1
*/

#include <vector>
#include <string>
#include <sstream>
#include <utility>


/**
 * @brief Splits a string into tokens based on a delimiter.
 *
 * This function takes a string and a delimiter as input and splits the string into tokens based on the delimiter.
 * The tokens are stored in a vector and returned.
 *
 * @param s The string to be split.
 * @param delimiter The delimiter used to split the string.
 * @return A vector of strings containing the tokens.
 */
std::vector<std::string> split_parameters(const std::string &s, const std::string &delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = s.find(delimiter);
    while (end != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    tokens.push_back(s.substr(start, end));
    return tokens;
}

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
	Client* client = _get_client(fd);

	std::vector<std::string> params = _split_buffer(buffer, " ");

	std::cout << params[0] << std::endl;
	std::cout << params[1] << std::endl;

	if (client->get_is_logged())
	{
		// Check if the command has the minimum number of parameters
		if (params.size() < 2)
		{
			_send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
			_reply_code = 461;
			return;
		}
	
		std::vector<std::string> receivers = split_parameters(params[0], ",");

		std::cout << receivers[0] << std::endl;
		
		// Validate channel and client inputs. Otherwise, return an error
		for (std::vector<std::string>::iterator it = receivers.begin(); it != receivers.end(); ++it)
		{
			// Check if first character is #
			if ((*it)[0] == '#')
			{
				// Check if the channel exists
				Channel *target_channel = this->_get_channel(*it);
				if (!target_channel)
				{
					_send_response(fd, ERR_NOSUCHCHANNEL(*it));
					_reply_code = 403;
					return;
				}

				// Check if the client is in the channel
				if (!target_channel->has_client(client))
				{
					_send_response(fd, ERR_NOTONCHANNEL(client->get_nickname()));
					_reply_code = 442;
					return;
				}
			}
			else
			{
				// Check if the receiver exists
				Client *target_client = this->_get_client(*it);
				if (!target_client)
				{
					_send_response(fd, ERR_NOSUCHNICK(std::string(""), client->get_nickname()));
					_reply_code = 401;
					return;
				}
			}
		}

		// Both channels and clients exist, now it's time to send the private message
		for (std::vector<std::string>::iterator it = receivers.begin(); it != receivers.end(); ++it)
		{
			if ((*it)[0] == '#')
			{
				Channel *target_channel = this->_get_channel(*it);
				
				std::cout << "1) THIS IS CHANNEL TARGET:" << target_channel->get_name() << std::endl;
				
				target_channel->broadcast(client, target_channel->get_name(), params[1]);
				
				//_send_response(fd, RPL_PRIVMSG(client->get_hostname(), target_channel->get_name(), params[1]));
			}
			else
			{
				// Check if the receiver exists
				Client *target_client = this->_get_client(*it);

				// Send the message to the receiver
				_send_response(target_client->get_fd(), RPL_PRIVMSG(client->get_hostname(), target_client->get_nickname(), params[1]));
			}
		}
	}
	else
	{
		_send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
		_reply_code = 451;
	}
	//_send_response(fd, RPL_PRIVMSG(client->get_hostname(), "ft_transcendence", "Hello, Carlos!"));
}