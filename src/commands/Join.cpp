/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:33:05 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/26 05:05:35 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: JOIN
 * Parameters: <channel>{,<channel>} [<key>{,<key>}]
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.1
*/

/**
 * @brief Handles the JOIN command received from the client.
 *
 * This method processes the JOIN command received from the client and sends a response
 * to the client indicating that the client has joined the channel.
 *
 * @param buffer The buffer containing the JOIN command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_join(const std::string &buffer, const int fd)
{
    Client* client = _get_client(fd);

    if (!client->get_is_logged()) {
        _send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
        return;
    }

    std::vector<std::string> params = _split_buffer(buffer, SPACE);
    if (params.size() < 1) {
        _send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
        return;
    }

    std::string joining_channel = params[0];
    if (joining_channel[0] != '#') {
        _send_response(fd, ERR_NOSUCHCHANNEL(joining_channel));
        return;
    }

    Channel *channel = _get_channel(joining_channel);
    if (!channel) {
        // Channel does not exist, create it
        // Se eu estou criando o canal eu devo me incluir e enviar a resposta para o cliente?
        channel = new Channel(joining_channel);
        _add_channel(channel);
    } else {
        if (channel->has_client(client)) {
            _send_response(fd, ERR_ALREADYREGISTERED(client->get_nickname()));
            return;
        }
    }


    // TODO: Para entrar em canal devemos seguir os itens abaixo?
    // Verificar se o cliente tem convite para entrar no canal
    // Verificar se o canal é protegido por senha
    // Verificar se o cliente tem a senha correta
    // Verificar se o canal está cheio
    // Verificar se o canal é apenas para convidados/invitados -> Deixei os invites na estrutura de cliente em std::vector<std::string> _channels_invited;
        
    
    
    // TODO: Implement channel keys
    // For now, assuming no channel keys are required

    // Join the channel
    channel->join(client);

    // Send successful join response
    //_send_response(fd, RPL_JOIN(client->get_nickname(), joining_channel));

    // TODO: Send welcome message to the channel
    // For now, assuming no welcome message is sent
}