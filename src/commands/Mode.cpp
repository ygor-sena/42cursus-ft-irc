/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:30:34 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/27 21:41:33 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#define MODE_CMD "MODE"

/*
 * Command: MODE
 * Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>]
 * Reference: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.3
*/

void _handler_invite_only_mode(Channel* channel, bool set);
void _handler_topic_restriction_mode(Channel* channel, bool set);
bool _parse_mode_command(const std::string &modes, char &mode, bool &set);
bool _handler_mode_flags(Channel* channel, Client *client, char mode, bool &set);
void _handle_limit_mode(Channel* channel, const std::string &argument, bool set);
void _handle_operator_privileges_mode(Channel* channel, Client *client, bool set);
void _handle_password_mode(Channel* channel, const std::string &argument, bool set);

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
    // Registra o comando MODE recebido
    std::cout << "MODE command received: " << buffer << std::endl;

    // Separar os argumentos do comando
	bool set;
	char mode = 0;
    std::istringstream iss(buffer);
    std::string chnl, modes, nick;
    iss >> chnl >> modes >> nick;

    Client* client = _get_client(nick);
    Channel* channel = _get_channel(chnl);

	if (!client->get_is_logged()) { // Verificar se o cliente está logado
        _send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
        _reply_code = 451;
    } else if (chnl.empty() || modes.empty() || nick.empty()) { // Verificar se todos os parâmetros foram fornecidos
        _send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
        _reply_code = 461;
    } else if (!channel) {
        _send_response(fd, ERR_NOSUCHCHANNEL(chnl));
        _reply_code = 403;
	} else if (!channel->is_channel_operator(nick)) { // Verificar se o cliente tem privilégios de operador no canal
        _send_response(fd, ERR_CHANOPRIVSNEEDED(chnl));
        _reply_code = 482;
    } else { // Tratar o comando MODE
        if (!_parse_mode_command(modes, mode, set)) {
            _send_response(fd, ERR_INVALIDMODEPARM(client->get_nickname(), mode));
            _reply_code = 696;
        }
        else { // Executar a ação correspondente ao modo
            if (!_handler_mode_flags(channel, client, mode, set)) {
				_send_response(fd, ERR_UNKNOWNMODE(client->get_nickname(), channel->get_name(), mode));
				_reply_code = 472;
			}
        }
    }
}

bool _handler_mode_flags(Channel* channel, Client *client, char mode, bool &set)
{
	switch (mode) {
		case 'i':
			_handler_invite_only_mode(channel, set); //Está funcionando
			break;
		case 't':
			_handler_topic_restriction_mode(channel, set);
			break;
		case 'k':
			_handle_password_mode(channel, "321", set); //Está funcionando
			break;
		case 'o':
			_handle_operator_privileges_mode(channel, client, set);
			break;
		case 'l':
			_handle_limit_mode(channel, "2", set); //Está funcionando
			break;
		default:
			return false;
	}
    return true;
}

bool _parse_mode_command(const std::string &modes, char &mode, bool &set)
{
	if (modes.size() != 2) {
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
		channel->remove_invite_only();
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
		channel->set_key(argument);
	}
	else {
		channel->remove_key();
	}
}

void _handle_operator_privileges_mode(Channel* channel, Client *client, bool set)
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
