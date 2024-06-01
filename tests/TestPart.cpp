/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestPart.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 08:56:56 by yde-goes          #+#    #+#             */
/*   Updated: 2024/05/31 22:22:04 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "Channel.hpp"
#include "Client.hpp"
#define private public
#include "Server.hpp"

Client* genericClient()
{
	Client* client = new Client();
	client->set_fd(6);
	client->set_username("soAndSoUser");
	client->set_nickname("soAndSoUser");
	client->set_password("password");
	client->set_buffer("");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	return client;
}

Client* toPartClient()
{
	Client* client = new Client();
	client->set_fd(4);
	client->set_username("toPartUser");
	client->set_nickname("toPartUser");
	client->set_password("password");
	client->set_buffer("");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	client->set_is_operator(false);
	return client;
}

/*
 * 1. O comando PART é recebido sem parâmetros suficientes.
 */
Test(PartCommand, part_without_enough_parameters)
{
	Client* client = genericClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_part("", client->get_fd());

	cr_assert(eq(int, server._reply_code, 461));
}

/*
 * 2. O comando PART é recebido e o cliente não está registrado.
 */
Test(PartCommand, part_not_registered)
{
	Client* client = genericClient();
	client->set_is_logged(false);

	Server server;
	server._clients.push_back(client);
	server._handler_client_part("#channel", client->get_fd());

	cr_assert(eq(int, server._reply_code, 451));
}

/*
 * 3. O comando PART é recebido e o canal não existe.
 */
Test(PartCommand, part_channel_does_not_exist)
{
	Client* client = genericClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_part("#nonExistent", client->get_fd());

	cr_assert(eq(int, server._reply_code, 403));
}

/*
 * 4. O comando PART é recebido e o cliente não está no canal.
 */
Test(PartCommand, part_not_on_channel)
{
	Client* client = genericClient();

	Channel* channel = new Channel("#channel");

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_part("#channel", client->get_fd());

	cr_assert(eq(int, server._reply_code, 442));
}

/*
 * 5. O comando PART é recebido e o cliente deixa o canal com sucesso.
 */
Test(PartCommand, part_successfully)
{
	Client* client = genericClient();

	Channel* channel = new Channel("#channel");
	channel->invite(client);

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_part("#channel", client->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, channel->get_clients_size(), 0));
}
