/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:30:34 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/26 20:46:35 by gilmar           ###   ########.fr       */
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
bool _handler_mode_command(const std::string &modes, char &mode, bool &set);
void _handle_limit_mode(Channel* channel, const std::string &argument, bool set);
void _handle_operator_privilages_mode(Channel* channel, Client *client, bool set);
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
    std::string channelName, modes, nickname;
    iss >> channelName >> modes >> nickname;

    // Obter o cliente e o canal associados aos argumentos
    Client* client = _get_client(nickname);
    Channel* channel = _get_channel(channelName);

    // Verificar se o cliente é nulo antes de acessar seus métodos
    if (!channel) {
        _send_response(fd, ERR_NOSUCHCHANNEL(channelName));
        _reply_code = 403;
    } else if (!client->get_is_logged()) { // Verificar se o cliente está logado
        _send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
        _reply_code = 451;
    } else if (channelName.empty() || modes.empty() || nickname.empty()) { // Verificar se todos os parâmetros foram fornecidos
        _send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
        _reply_code = 461;
    } else if (!channel->is_channel_operator(nickname)) { // Verificar se o cliente tem privilégios de operador no canal
        _send_response(fd, ERR_CHANOPRIVSNEEDED(channelName));
        _reply_code = 482;
    } else { // Tratar o comando MODE
        if (!_handler_mode_command(modes, mode, set)) {
            _send_response(fd, ERR_INVALIDMODEPARM(client->get_nickname(), mode));
            _reply_code = 696;
        }
        else { // Executar a ação correspondente ao modo
            switch (mode) {
                case 'i':
                    _handler_invite_only_mode(channel, set);
                    break;
                case 't':
                    _handler_topic_restriction_mode(channel, set);
                    break;
                case 'k':
                    _handle_password_mode(channel, nickname, set);
                    break;
                case 'o':
                    _handle_operator_privilages_mode(channel, client, set);
                    break;
                case 'l':
                    _handle_limit_mode(channel, nickname, set);
                    break;
                default:
                    _send_response(fd, ERR_UNKNOWNMODE(nickname, channelName, mode));
                    _reply_code = 472;
                    break;
            }
        }
    }
}

bool _handler_mode_command(const std::string &modes, char &mode, bool &set)
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
