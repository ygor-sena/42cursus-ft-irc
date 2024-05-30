/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestNick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:03:34 by yde-goes          #+#    #+#             */
/*   Updated: 2024/05/30 16:48:25 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "Client.hpp"
#include "Channel.hpp"
#define private public
#include "Server.hpp"

/*
 * Cenários de Teste:
 * 1. O comando NICK é recebido sem parâmetros suficientes.
 * 2. O comando NICK é recebido e o cliente não está registrado.
 * 3. O comando NICK é recebido e o nickname do cliente é inválido.
 * 4. O comando NICK é recebido e o cliente está registrado.
 * 5. O comando NICK é recebido e o nickname do cliente está em uso.
 * 6. O comando NICK é recebido e o nickname do cliente é alterado.
 * 7. O comando NICK é recebido e o cliente está pronto para logar.
*/

Client *mockClient()
{
	Client *client = new Client();
	client->set_fd(4);
	client->set_username("Username");
	client->set_nickname("Nickname");
	client->set_password("Password");
	client->set_buffer("NICK Username");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	return client;
}

/*
 * 1. O comando NICK é recebido sem parâmetros suficientes.
*/
Test(NickCommand, err_needmoreparams)
{
	Client* client = mockClient();

	Server server;

	server._clients.push_back(*client);
	server._handler_client_nickname("", 4);
	cr_assert(eq(int, server._reply_code, 461));
}

/*
 * 2. O comando NICK é recebido e o cliente não está registrado.
*/
Test(NickCommand, err_notregistered)
{
	Client* client = mockClient();
	client->set_is_authenticated(false);

	Server server;

	server._clients.push_back(*client);
	server._handler_client_nickname(client->get_nickname(), 4);
	cr_assert(eq(int, server._reply_code, 451));
}

/*
 * 3. O comando NICK é recebido e o nickname do cliente é inválido.
*/
Test(NickCommand, err_erroneusnick_1)
{
	Client* client = mockClient();
	client->set_is_authenticated(true);

	Server server;

	server._clients.push_back(*client);
	server._handler_client_nickname("new_nickname", 4);
	cr_assert(eq(int, server._reply_code, 432));
}

Test(NickCommand, err_erroneusnick_2)
{
	Client* client = mockClient();
	client->set_is_authenticated(true);

	Server server;

	server._clients.push_back(*client);
	server._handler_client_nickname("$Gilmar", 4);
	cr_assert(eq(int, server._reply_code, 432));
}

Test(NickCommand, err_erroneusnick_3)
{
	Client* client = mockClient();
	client->set_is_authenticated(true);

	Server server;

	server._clients.push_back(*client);
	server._handler_client_nickname("Ygor%", 4);
	cr_assert(eq(int, server._reply_code, 432));
}

Test(NickCommand, err_erroneusnick_4)
{
	Client* client = mockClient();
	client->set_is_authenticated(true);

	Server server;

	server._clients.push_back(*client);
	server._handler_client_nickname("Car#los", 4);
	cr_assert(eq(int, server._reply_code, 432));
}

/*
 * 4. O comando NICK é recebido e o cliente está registrado.
*/
Test(NickCommand, success_setnickname)
{
	Client* client = mockClient();
	client->set_is_authenticated(true);

	Server server;

	server._clients.push_back(*client);
	server._handler_client_nickname("newNickname", 4);
	cr_assert(eq(int, server._reply_code, 200));
}

/*
 * 5. O comando NICK é recebido e o nickname do cliente está em uso.
*/
Test(NickCommand, err_nickinuse)
{
	Client* client = mockClient();
	client->set_is_authenticated(true);
	
	Client* client1 = mockClient();
	client1->set_fd(5);
	client1->set_is_authenticated(true);

	Server server;

	server._clients.push_back(*client);
	server._clients.push_back(*client1);
	server._handler_client_nickname(client1->get_nickname(), 4);
	cr_assert(eq(int, server._reply_code, 433));
}

/*
 * 6. O comando NICK é recebido e o nickname do cliente é alterado.
*/
Test(NickCommand, success_changenickname)
{
	Client* client = mockClient();
	client->set_is_authenticated(true);

	Server server;

	server._clients.push_back(*client);
	server._handler_client_nickname("newNickname", 4);
	cr_assert(eq(int, server._reply_code, 200));
}

/*
 * 7. O comando NICK é recebido e o cliente está pronto para logar.
*/
Test(NickCommand, success_readytologin)
{
	Client* client = mockClient();
	client->set_is_authenticated(true);
	client->set_is_logged(false);

	Server server;

	server._clients.push_back(*client);
	server._handler_client_nickname("Nickname", 4);
	cr_assert(eq(int, server._reply_code, 001));
}
