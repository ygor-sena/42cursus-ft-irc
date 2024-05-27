/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:33:05 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/26 22:51:03 by gilmar           ###   ########.fr       */
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
        // Quem cria o canal já é moodn então ele é único a adicionar outrs mood e qlqr outra coisa.
        channel = new Channel(joining_channel);
        _add_channel(channel);
        channel->join(client);
        channel->set_channel_operator(client);
        return;
    } else {
        if (channel->has_client(client)) {
            _send_response(fd, ERR_ALREADYREGISTERED(client->get_nickname()));
            return;
        }
    }

    //Verifica se o canal está full e envia mensagem de erro
    if (channel->is_channel_full()) {
        _send_response(fd, ERR_CHANNELISFULL(client->get_nickname(), joining_channel));
        return;
    }

    //TODO: Ygor, não me julgue pelo código criminoso que coloquei abaixo, eu sei que está errado, mas não consegui pensar em outra forma de fazer isso
    // pq estou com sono e não consigo pensar direito, mas amanhã eu arrumamos isso. Eu não testei mas acredito estar funcional...
    // precisa verificar os retornos do servidor para verificar se está tudo ok.

    // Verifica se o canal é apenas para convidados/invitados
    if (channel->is_channel_invite_only()) {
        // Verifica se o cliente tem convite para entrar no canal
        if (!client->is_channel_invited(joining_channel)) {
            _send_response(fd, ERR_INVITEONLYCHAN(client->get_nickname(), joining_channel));
            return;
        } else {
            // O cliente possui o convite, agr precisamos validar se precisa de senha para entrar canal.
            if (channel->has_key()) {
                // Verifica se o cliente tem a senha correta
                if (params.size() < 2) {
                    _send_response(fd, ERR_BADCHANNELKEY(client->get_nickname(), joining_channel));
                    return;
                } else {
                    std::string channel_key = params[1];
                    if (channel_key != channel->get_channel_key()) {
                        _send_response(fd, ERR_BADCHANNELKEY(client->get_nickname(), joining_channel));
                        return;
                    } else {
                        channel->join(client);
                        return;
                    }
                }
            } else {
                channel->join(client);
                return;
            }
        }
    } else {
        channel->join(client);
    }

    // Registra o comando JOIN recebido
    std::cout << "JOIN command received from client " << buffer << std::endl;
}