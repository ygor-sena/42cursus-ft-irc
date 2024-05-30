/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:31:16 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/30 14:39:36 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"

/*
 * Command: INVITE
 * Parameters: <nickname> <channel>
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.7
 * Example: INVITE Wiz #Twilight_Zone
*/

/*
 * Cenários de Teste:
 * 1. O comando INVITE é recebido sem parâmetros suficientes.
 * 2. O comando INVITE é recebido e o cliente não está registrado.
 * 3. O comando INVITE é recebido e o canal não existe.
 * 4. O comando INVITE é recebido e o cliente não está no canal.
 * 5. O comando INVITE é recebido e o cliente não é operador do canal para convidar um cliente.
 * 6. O comando INVITE é recebido e o cliente convidado não existe.
 * 7. O comando INVITE é recebido e o cliente convidado já está no canal.
 * 8. O comando INVITE é recebido e o cliente convidado é convidado para o canal.
 * 9. O comando INVITE é recebido e o cliente não está logado.
 * 10. O comando INVITE é recebido e o cliente está logado.
 * 
*/ 

/**
 * @brief Handles the INVITE command received from the client.
 *
 * This method processes the INVITE command received from the client and sends a response
 * to the client indicating that the client has been invited to the channel.
 *
 * @param buffer The buffer containing the INVITE command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_invite(const std::string &buffer, const int fd)
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
    std::vector<std::string> params = _split_buffer(buffer, " ");
    if (params.size() < 2) {
        _send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
        _reply_code = 461;
        return;
    }

    // Extrai o apelido do cliente convidado e o nome do canal
    std::string target_nickname = params[0];
    std::string target_channel = params[1];

    // Verifica se o canal existe
    Channel *channel = _get_channel(target_channel);
    if (!channel) {
        _send_response(fd, ERR_NOSUCHCHANNEL(target_channel));
        _reply_code = 403;
        return;
    }

    // Verifica se o cliente está no canal
    if (!channel->has_client(client)) {
        _send_response(fd, ERR_NOTONCHANNEL(target_channel));
        _reply_code = 442;
        return;
    }

    // Verifica se o cliente é um operador do canal
    if (!channel->is_channel_operator(client->get_nickname())) {
        _send_response(fd, ERR_NOPRIVILEGES(client->get_nickname()));
        _reply_code = 481;
        return;
    }

    // Verifica se o cliente convidado existe
    Client *invited_client = _get_client(target_nickname);
    if (!invited_client) {
        _send_response(fd, ERR_NOSUCHNICK(target_channel, target_nickname));
        _reply_code = 401;
        return;
    }

    // Verifica se o cliente convidado já está no canal
    if (channel->has_client(invited_client)) {
        _send_response(fd, ERR_USERONCHANNEL(target_nickname, target_channel));
        _reply_code = 443;
        return;
    }

    // Envia o convite ao cliente convidado
    invited_client->add_channel_invited(target_channel);

    // Envia a resposta de convite ao cliente que enviou o convite
    _send_response(fd, RPL_INVITING(client->get_hostname(), target_channel, client->get_nickname(), target_nickname));
    _reply_code = 200;

    // Registra o comando INVITE recebido
    std::cout << "INVITE command received from client " << buffer << std::endl;
}
