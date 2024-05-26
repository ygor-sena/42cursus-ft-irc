/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:32:16 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/26 20:42:13 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#define USER_CMD "USER"

/*
 * Command: USER
 * Parameters: <username>
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.3
*/

/**
 * @brief Handles the USERNAME command received from the client.
 *
 * This method processes the USERNAME command received from the client and sends a response
 * to the client indicating that the client's username has been set.
 *
 * @param username The username to be assigned to the client.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_username(const std::string &buffer, const int fd)
{
    // Registra o comando USER recebido
    std::cout << "USER command received: " << buffer << std::endl;
    
    Client* client = _get_client(fd);
    
    if (buffer.size() < 5) {
        // Verifica se há parâmetros suficientes
        _send_response(fd, ERR_NEEDMOREPARAMS(std::string("*")));
        _reply_code = 461;
    } else if (!client || !client->get_is_authenticated()) {
        // Verifica se o cliente está autenticado
        _send_response(fd, ERR_NOTREGISTERED(std::string("*")));
        _reply_code = 451;
    } else if (!client->get_username().empty()) {
        // Verifica se o nome de usuário já está registrado
        _send_response(fd, ERR_ALREADYREGISTERED(client->get_nickname()));
        _reply_code = 462;
    } else {
        // Define o nome de usuário do cliente e verifica se está pronto para login
        client->set_username(buffer);
        if (_client_is_ready_to_login(fd)) {
            client->set_is_logged(fd);
            _send_response(fd, RPL_CONNECTED(client->get_nickname()));
            _reply_code = 001;
        } else {
            _reply_code = 200;
        }
    }
}
