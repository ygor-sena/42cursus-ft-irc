/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:30:34 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/26 04:37:36 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#define MODE_CMD "MODE"

/*
 * Command: MODE
 * Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>]
 * Reference: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.3
*/

bool _parser_mode_command(const std::string &modes, char &mode, bool &set);
void _handler_invite_only_mode(Channel* channel, bool set);
void _handler_topic_restriction_mode(Channel* channel, bool set);
void _handle_password_mode(Channel* channel, const std::string &argument, bool set);
void _handle_operator_privilages_mode(Channel* channel, Client *client, bool set);
void _handle_limit_mode(Channel* channel, const std::string &argument, bool set);

/**
 * @brief Handles the MODE command received from the client.
 *
 * This method processes the MODE command received from the client and sends a response
 * to the client indicating the channel modes supported by the server.
 *
 * @param buffer The buffer containing the MODE command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_mode(const std::string &buffer, const int fd)
{
	(void)fd;
	bool set;
	char mode;
	std::istringstream iss(buffer);
	std::string channel_name, modes, argument;
	iss >> channel_name >> modes >> argument;
	
	Client* client = _get_client(argument);

	if (buffer == MODE_CMD) {
		_send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
		return;
	}
	
	Channel* channel = _get_channel(channel_name);
	if (channel == NULL) {
        _send_response(fd, ERR_NOSUCHCHANNEL(channel_name));
        return ;
    }
	
	if (!_parser_mode_command(modes, mode, set)) {
		return ;
	}

	switch (mode) {
		case 'i':
			_handler_invite_only_mode(channel, set);
			break;
		case 't':
			_handler_topic_restriction_mode(channel, set);
			break;
		case 'k':
			_handle_password_mode(channel, argument, set);
			break;
		case 'o':
			_handle_operator_privilages_mode(channel, client, set);
			break;
		case 'l':
			_handle_limit_mode(channel, argument, set);
			break;
        default:
            std::cerr << "Unknown mode: " << mode << std::endl;
            break;
	}

	// Registra o comando MODE recebido
    std::cout << "MODE command received from client " << buffer << std::endl;
}

bool _parser_mode_command(const std::string &modes, char &mode, bool &set)
{
    if (modes.size() < 2) {
        std::cerr << "Invalid modes string: " << modes << std::endl;
        return false;
    }
	
    set = (modes[0] == '+');
    mode = modes[1];
	
    return true;
}

void _handler_invite_only_mode(Channel* channel, bool set)
{
	if (set) {
		channel->set_invite_only();
	}
	else {
		channel->set_invite_only();
	}
}


void _handler_topic_restriction_mode(Channel* channel, bool set)
{
	if (set) {
		channel->set_topic_restriction();
	}
	else {
		channel->remove_topic_restriction();
	}
}

void _handle_password_mode(Channel* channel, const std::string &argument, bool set)
{
	if (set) {
		channel->set_password(argument);
	}
	else {
		channel->remove_password();
	}
}

void _handle_operator_privilages_mode(Channel* channel, Client *client, bool set)
{
	if (set) {
		if (client) {
			channel->set_channel_operator(client);
		}
	}
	else {
		if (client) {
			channel->remove_channel_operator(client);
		}
	}
}

void _handle_limit_mode(Channel* channel, const std::string &argument, bool set)
{
	if (set) {
		channel->set_limit(std::atoi(argument.c_str()));
	}
	else {
		channel->remove_limit();
	}
}
