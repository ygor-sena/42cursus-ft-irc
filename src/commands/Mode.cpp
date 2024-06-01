/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:30:34 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/31 21:53:09 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#define MODE_CMD "MODE"

/*
 * Command: MODE
 * Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>]
 * Reference: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.3
 */

/*
 * Cenários de Teste:
 * 1. O comando MODE é recebido sem parâmetros suficientes.
 * 2. O comando MODE é recebido e o canal não existe.
 * 3. O comando MODE é recebido e o cliente não tem privilégios de operador no
 * canal.
 * 4. O comando MODE é recebido e o cliente tem privilégios de operador no
 * canal.
 * 5. O comando MODE é recebido e o modo do canal é alterado com sucesso.
 *
 */

/**
 * @brief Handles the MODE command received from the client.
 *
 * This method processes the MODE command received from the client and sends a
 * response to the client indicating the channel modes supported by the server.
 *
 * @param buffer The buffer containing the MODE command parameters.
 * @param fd The file descriptor associated with the client that sent the
 * command.
 */
void Server::_handler_client_mode(const std::string& buffer, const int fd)
{
	// Registra o comando MODE recebido
	std::cout << "MODE command received: " << buffer << std::endl;

	// Separar os argumentos do comando
	std::istringstream iss(buffer);
	std::string chnl, modes, arg;

	// Verificar e extrair os parâmetros de acordo com o padrão esperado
	iss >> chnl >> modes;
	if (!chnl.empty() && !modes.empty())
	{
		iss >> arg;
	}

	Client* admin = _get_client(fd);
	Channel* channel = _get_channel(chnl);

	// Verificar se todos os parâmetros obrigatórios foram fornecidos
	if (chnl.empty() || modes.empty())
	{
		_send_response(fd, ERR_NEEDMOREPARAMS(admin->get_nickname()));
		_reply_code = 461;
		return;
	}

	// Verificar se o canal existe
	if (!channel)
	{
		_send_response(fd, ERR_NOSUCHCHANNEL(chnl));
		_reply_code = 403;
		return;
	}

	// Verificar se o cliente tem privilégios de operador no canal
	if (!channel->is_channel_operator(admin->get_nickname()))
	{
		_send_response(fd, ERR_CHANOPRIVSNEEDED(chnl));
		_reply_code = 482;
		return;
	}

	// Tratar o comando MODE
	if (!_parse_mode_command(modes, channel, _get_client(arg), arg, fd))
	{
		//_send_response(fd, ERR_UNKNOWNMODE(_get_client(fd)->get_nickname(), channel->get_name(), modes[1]));
		// _send_response(fd,
		// 			   ERR_INVALIDMODEPARM(admin->get_nickname(), modes[1]));
		//_reply_code = 696;
		return;
	}

  // TODO: VERIFICAR SE EXISTE UMA RESPOSTA PARA ESTA SITUAÇÃO, NO MOMENTO
	// ESTÁ COMO 200 MAS DEVE HAVER OUTRO REPLY.

	// TODO: VERIFICAR SE EXISTE UMA RESPOSTA CORRETA.
	// Enviar resposta de sucesso ao cliente
	//_send_response(fd, RPL_UMODEIS(admin->get_nickname(), chnl, modes));
	//_reply_code = 324;

	_reply_code = 200;
}

bool Server::_parse_mode_command(const std::string& modes, Channel* channel,
								 Client* client, const std::string& arg,
								 const int fd)
{
	bool set = false;
	char mode = 0;

	for (size_t i = 0; i < modes.size(); ++i)
	{
		if (modes[i] == '+' || modes[i] == '-')
		{
			set = (modes[i] == '+');
		}
		else
		{
			mode = modes[i];

			// if (client == NULL && (mode == 'o' || mode == 'i')) // Se o cliente não for encontrado
			// {
			// 	return false;
			// }

			if (!_handler_mode_flags(channel, client, mode, set, arg, fd))
			{
				_send_response(
					fd,
					ERR_UNKNOWNMODE(
						_get_client(fd)->get_nickname(), channel->get_name(), mode));
				_reply_code = 472;
				return false;
			}
		}
	}

	std::string signal = set ? "+" : "-";
	_send_response(fd, RPL_UMODEIS(client->get_nickname(), client->get_hostname(), channel->get_name(), signal, mode, arg));
	return true;
}

bool Server::_handler_mode_flags(Channel* channel, Client* client, char mode,
								 bool set, const std::string& arg, const int fd)
{
	switch (mode)
	{
	case 'i':
		_handler_invite_only_mode(channel, set);
		break;
	case 't':
		_handler_topic_restriction_mode(channel, set);
		break;
	case 'k':
		_handle_password_mode(channel, arg, set);
		break;
	case 'o':
		_handle_operator_privileges_mode(channel, client, set);
		break;
	case 'l':
		_handle_limit_mode(channel, client, arg, set, fd);
		break;
	default:
		return false;
	}
	return true;
}

void Server::_handler_invite_only_mode(Channel* channel, bool set)
{
	if (set)
	{
		channel->set_invite_only();
	}
	else
	{
		channel->remove_invite_only();
	}
}

void Server::_handler_topic_restriction_mode(Channel* channel, bool set)
{
	if (set)
	{
		channel->set_topic_restriction();
	}
	else
	{
		channel->remove_topic_restriction();
	}
}

void Server::_handle_password_mode(Channel* channel,
								   const std::string& argument, bool set)
{
	if (set)
	{
		channel->set_key(argument);
	}
	else
	{
		channel->remove_key();
	}
}

void Server::_handle_operator_privileges_mode(Channel* channel, Client* client,
											  bool set)
{
	if (set)
	{
		if (client)
		{
			channel->set_channel_operator(client);
		}
	}
	else
	{
		if (client)
		{
			channel->remove_channel_operator(client);
		}
	}
}

void Server::_handle_limit_mode(Channel* channel, Client* client,
								const std::string& argument, bool set,
								const int fd)
{
	if (set)
	{
		if (!argument.empty())
		{
			channel->set_limit(std::atoi(argument.c_str()));
		}
		else
		{
			// Respond with error if limit is not provided
			_send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
			_reply_code = 461;
		}
	}
	else
	{
		channel->remove_limit();
	}
}