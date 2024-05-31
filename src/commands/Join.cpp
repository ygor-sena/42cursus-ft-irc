/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:33:05 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/30 14:53:41 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: JOIN
 * Parameters: <channel>{,<channel>} [<key>{,<key>}]
 * Link: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.1
 */

/*
 * Cenários de Teste:
 * 1. O comando JOIN é recebido sem parâmetros suficientes.
 * 2. O comando JOIN é recebido e o cliente não está registrado.
 * 3. O comando JOIN é recebido e o nome do canal é inválido.
 * 4. O comando JOIN é recebido e o cliente já está no canal.
 * 5. O comando JOIN é recebido e o canal está cheio.
 * 6. O comando JOIN é recebido e o canal é apenas para convidados.
 * 7. O comando JOIN é recebido e o canal tem uma chave inválida.
 * 8. O comando JOIN é recebido e o cliente é adicionado ao canal.
 * 9. O comando JOIN é recebido e o cliente não está logado.
 * 10. O comando JOIN é recebido e o cliente está logado.
 *
 */

/**
 * @brief Handles the JOIN command received from the client.
 *
 * This method processes the JOIN command received from the client and sends a
 * response to the client indicating that the client has joined the channel.
 *
 * @param buffer The buffer containing the JOIN command parameters.
 * @param fd The file descriptor associated with the client that sent the
 * command.
 */
void Server::_handler_client_join(const std::string& buffer, const int fd)
{
	Client* client = _get_client(fd);

	// Verificar se o cliente está logado (descomentado caso seja necessário)
	if (!client->get_is_logged())
	{
		_send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
		_reply_code = 451;
		return;
	}

	// Separar os parâmetros do comando
	std::vector<std::string> params = _split_buffer(buffer, SPACE);
	if (params.size() < 1)
	{
		_send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
		_reply_code = 461;
		return;
	}

	std::string joining_channel = params[0];
	std::string channel_key = (params.size() > 1) ? params[1] : "";

	// Verificar se o nome do canal é válido
	if (joining_channel[0] != '#')
	{
		_send_response(fd, ERR_NOSUCHCHANNEL(joining_channel));
		_reply_code = 403;
		return;
	}

	Channel* channel = _get_channel(joining_channel);
	if (!channel)
	{
		channel = new Channel(joining_channel);
		_add_channel(channel);
		channel->join(client);
		channel->set_channel_operator(client);
		return;
	}

	// Verificar se o cliente já está no canal
	if (channel->has_client(client))
	{
		_send_response(fd, ERR_ALREADYREGISTERED(client->get_nickname()));
		_reply_code = 462;
		return;
	}

	// Verificar se o canal está cheio
	if (channel->is_channel_full())
	{
		_send_response(
			fd, ERR_CHANNELISFULL(client->get_nickname(), joining_channel));
		_reply_code = 422;
		return;
	}

	// Verificar se o canal é apenas para convidados
	if (channel->is_channel_invite_only() &&
		!client->is_channel_invited(joining_channel))
	{
		_send_response(
			fd, ERR_INVITEONLYCHAN(client->get_nickname(), joining_channel));
		_reply_code = 473;
		return;
	}

	// Verificar se o canal tem uma chave (senha) e se a chave fornecida é
	// válida
	if (channel->has_key() && channel_key != channel->get_channel_key())
	{
		_send_response(
			fd, ERR_BADCHANNELKEY(client->get_nickname(), joining_channel));
		_reply_code = 475;
		return;
	}

	// Adicionar o cliente ao canal
	channel->join(client);
	_reply_code = 200;

	// Registra o comando JOIN recebido
	std::cout << "JOIN command received from client " << buffer << std::endl;
}