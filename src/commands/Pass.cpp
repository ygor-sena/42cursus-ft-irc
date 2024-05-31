/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:32:39 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/30 14:49:57 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: PASS
 * Parameters: <password>
 * Reference: https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.1
 */

/*
 * Cenários de Teste:
 * 1. O comando PASS é recebido sem parâmetros suficientes.
 * 2. O comando PASS é recebido e o cliente já está autenticado.
 * 3. O comando PASS é recebido e a senha do cliente está incorreta.
 * 4. O comando PASS é recebido e a senha do cliente está correta.
 * 5. O comando PASS é recebido e o cliente não está logado.
 * 6. O comando PASS é recebido e o cliente está logado.
 * 7. O comando PASS é recebido e o cliente não está autenticado.
 * 8. O comando PASS é recebido e o cliente está autenticado.
 *
 */

/**
 * @brief Handles the PASSWORD command received from the client.
 *
 * This method processes the PASSWORD command received from the client and sends
 * a response to the client indicating that the client's password has been set.
 *
 * @param password The password to be assigned to the client.
 * @param fd The file descriptor associated with the client that sent the
 * command.
 */
void Server::_handler_client_password(const std::string& buffer, const int fd)
{
	// Registra o comando PASS recebido
	std::cout << "PASS command received: " << buffer << std::endl;

	Client* client = _get_client(fd);

	if (buffer.empty())
	{
		_send_response(fd, ERR_NEEDMOREPARAMS(std::string("*")));
		_reply_code = 461;
	}
	else if (client->get_is_authenticated())
	{
		_send_response(fd, ERR_ALREADYREGISTERED(std::string("*")));
		_reply_code = 462;
	}
	else if (_password != buffer)
	{
		_send_response(fd, ERR_INCORPASS(std::string("*")));
		_reply_code = 464;
	}
	else
	{
		client->set_is_authenticated(true);
		_reply_code = 200;
	}
}
