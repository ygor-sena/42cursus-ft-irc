/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:30:20 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/30 17:00:05 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: PART
 * Parameters: <channel>{,<channel>}
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.2
*/

/*
 * Test Cases:
 * 1. The PART command is received without enough parameters.
 * 2. The PART command is received and the channel does not exist.
 * 3. The PART command is received and the client is not in the channel.
 * 4. The PART command is received and the client leaves the channel successfully.
*/

/**
 * @brief Handles the PART command received from the client.
 *
 * This method processes the PART command received from the client and sends a response
 * to the client indicating that the client has left the channel.
 *
 * @param buffer The buffer containing the PART command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_part(const std::string &buffer, const int fd)
{
    // Obtém o cliente associado ao descritor de arquivo (fd)
    Client* client = _get_client(fd);

    // Verifica se o cliente está registrado e autenticado
    if (!client->get_is_logged()) {
        _send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
        _reply_code = 451;
        return;
    }

    // Divide o buffer em parâmetros
    std::vector<std::string> params = _split_buffer(buffer, SPACE);
    if (params.size() < 1) {
        _send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
        _reply_code = 461;
        return;
    }

    // Extrai o nome do canal a ser deixado
    std::string channel_name = params[0];

    Channel *channel = _get_channel(channel_name);
    if (!channel) {
        _send_response(fd, ERR_NOSUCHCHANNEL(channel_name));
        _reply_code = 403;
        return;
    }

    // Verifica se o cliente está no canal
    if (!channel->has_client(client)) {
        _send_response(fd, ERR_NOTONCHANNEL(channel_name));
        _reply_code = 442;
        return;
    }

    // Remove o cliente do canal
    channel->part(client);

    // Envia uma resposta ao cliente
    _send_response(fd, RPL_PART(client->get_hostname(), channel_name, client->get_nickname()));
    _reply_code = 200;

    // Registra o comando PART recebido
    std::cout << "PART command received from client " << buffer << std::endl;
}
