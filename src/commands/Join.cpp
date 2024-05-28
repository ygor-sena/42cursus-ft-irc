/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:33:05 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/27 21:41:16 by gilmar           ###   ########.fr       */
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
    if (!channel) { // Está funcionando.
        // Channel does not exist, create it
        // Se eu estou criando o canal eu devo me incluir e enviar a resposta para o cliente?
        // Quem cria o canal já é moodn então ele é único a adicionar outrs mood e qlqr outra coisa.
        channel = new Channel(joining_channel);
        _add_channel(channel);
        channel->join(client);
        channel->set_channel_operator(client);
        _send_response(fd, "canal criado\n\r");
        return;
    } else {
        bool teste = channel->has_client(client);
        if (teste) { // Está funcionando
            _send_response(fd, ERR_ALREADYREGISTERED(client->get_nickname()));
            return;
        }
    }

    //Verifica se o canal está full e envia mensagem de erro
    if (channel->is_channel_full()) { // Está funcionando
        _send_response(fd, ERR_CHANNELISFULL(client->get_nickname(), joining_channel));
        return;
    }

    // Verifica se o canal é apenas para convidados/invitados
    if (channel->is_channel_invite_only()) { // Está funcionando.
        // Verifica se o cliente tem convite para entrar no canal
        if (!client->is_channel_invited(joining_channel)) {
            _send_response(fd, ERR_INVITEONLYCHAN(client->get_nickname(), joining_channel));
            return;
        } else {
            // O cliente possui o convite, agr precisamos validar se precisa de senha para entrar canal.
            if (channel->has_key()) { // Verifica se o cliente tem a senha correta
                if (params.size() < 2) { // ESTÁ FUNCIONANDO
                    _send_response(fd, ERR_BADCHANNELKEY(client->get_nickname(), joining_channel));
                    return;
                } else { 
                    // ESTÁ FUNCIONANDO
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
        if (channel->has_key()) {
            // Sempre vai cair nesta condição quando o canal não for apenas para convidados/invitados
            if (params.size() < 2) { // ESTÁ FUNCIONANDO
                _send_response(fd, ERR_BADCHANNELKEY(client->get_nickname(), joining_channel));
                return;
            } else { 
                // ESTÁ FUNCIONANDO
                std::string channel_key = params[1];
                if (channel_key != channel->get_channel_key()) {
                    _send_response(fd, ERR_BADCHANNELKEY(client->get_nickname(), joining_channel));
                    return;
                } else {
                    channel->join(client);
                    _reply_code = 200;
                    return;
                }
            }
        }
        else {
            channel->join(client);
            _reply_code = 200;
            return;
        }
    }

    // Registra o comando JOIN recebido
    std::cout << "JOIN command received from client " << buffer << std::endl;
}