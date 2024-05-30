/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:32:16 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/30 15:19:14 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#define USER_CMD "USER"

/*
 * Command: USER
 * Parameters: <username>
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.3
*/


/*
 * Cenários de Teste:
 * 1. O comando USER é recebido sem parâmetros suficientes.
 * 2. O comando USER é recebido e o cliente não está registrado.
 * 3. O comando USER é recebido e o cliente já está registrado.
 * 4. O comando USER é recebido e o cliente está pronto para fazer login.
 * 5. O comando USER é recebido e o cliente não está pronto para fazer login.
 * 6. O comando USER é recebido e o cliente já está logado.
 * 7. O comando USER é recebido e o cliente não está autenticado.
 * 8. O comando USER é recebido e o cliente está autenticado.
 * 
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
    
    if (buffer.empty()) {
        _send_response(fd, ERR_NEEDMOREPARAMS(std::string("*")));
        _reply_code = 461;
    } else if (!client || !client->get_is_authenticated()) {
        _send_response(fd, ERR_NOTREGISTERED(std::string("*")));
        _reply_code = 451;
    } else if (!client->get_username().empty()) {
        _send_response(fd, ERR_ALREADYREGISTERED(client->get_nickname()));
        _reply_code = 462;
    } else {
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
