/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestUser.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:03:11 by yde-goes          #+#    #+#             */
/*   Updated: 2024/05/31 11:20:53 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "Channel.hpp"
#include "Client.hpp"
#define private public
#include "Server.hpp"

/*
 * Cenários de Teste:
 * 1. O comando USER é recebido sem parâmetros suficientes.
 * 2. O comando USER é recebido e o cliente não está registrado.
 * 3. O comando USER é recebido e o cliente já está registrado.
 * 4. O comando USER é recebido e o cliente está pronto para fazer login.
 * 5. O comando USER é recebido e o cliente não está pronto para fazer login.
 *
 */

Client* mockClient()
{
	Client* client = new Client();
	client->set_fd(4);
	client->set_username("Username");
	client->set_nickname("Nickname");
	client->set_password("Password");
	client->set_buffer("USER Username");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	return client;
}

/*
 * 1. O comando USER é recebido sem parâmetros suficientes.
 */
Test(UserCommand, err_needmoreparams)
{
	Client* client = mockClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_username("", client->get_fd());

	cr_assert(eq(int, server._reply_code, 461));
}

/*
 * 2. O comando USER é recebido e o cliente não está registrado.
 */
Test(UserCommand, err_notregistered)
{
	Client* client = mockClient();
	client->set_is_authenticated(false);
	client->set_is_logged(false);

	Server server;
	server._clients.push_back(client);
	server._handler_client_username(client->get_username(), client->get_fd());

	cr_assert(eq(int, server._reply_code, 451));
}

/*
 * 3. O comando USER é recebido e o cliente já está registrado.
 */
Test(UserCommand, err_alreadyregistered)
{
	Client* client = mockClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_username(client->get_username(), client->get_fd());

	cr_assert(eq(int, server._reply_code, 462));
}

/*
 * 4. O comando USER é recebido e o cliente está pronto para fazer login.
 */
Test(UserCommand, success_readytologin)
{
	Client* client = mockClient();
	client->set_username("");
	client->set_is_authenticated(true);
	client->set_is_logged(false);

	Server server;
	server._clients.push_back(client);
	server._handler_client_username("Username", client->get_fd());

	cr_assert(eq(int, server._reply_code, 001));
}

/*
 * 5. O comando USER é recebido e o cliente não está pronto para fazer login.
 */
Test(UserCommand, success_notreadytologin)
{
	Client* client = mockClient();
	client->set_username("");
	client->set_nickname("");
	client->set_is_authenticated(true);
	client->set_is_logged(false);

	Server server;
	server._clients.push_back(client);
	server._handler_client_username("Username", client->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
}
