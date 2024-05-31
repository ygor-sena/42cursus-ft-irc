/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestPass.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:27:49 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/31 11:24:21 by yde-goes         ###   ########.fr       */
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

Client* mockClient()
{
	Client* client = new Client();
	client->set_fd(4);
	client->set_username("Username");
	client->set_nickname("Nickname");
	client->set_password("Password");
	client->set_buffer("Pass password");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	client->set_is_operator(false);
	return client;
}

/*
 * 1. O comando PASS é recebido sem parâmetros suficientes.
 */
Test(ServerHandlerClientPassword, TestPassWithoutEnoughParameters)
{
	Client* client = mockClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_password("", client->get_fd());

	cr_assert_eq(server._reply_code, 461);
}

/*
 * 2. O comando PASS é recebido e o cliente já está autenticado.
 */
Test(ServerHandlerClientPassword, TestPassClientIsAuthenticated)
{
	Client* client = mockClient();
	client->set_is_authenticated(true);

	Server server;
	server._password = "Password";
	server._clients.push_back(client);
	server._handler_client_password("Password", client->get_fd());

	cr_assert_eq(server._reply_code, 462);
}

/*
 * 3. O comando PASS é recebido e a senha do cliente está incorreta.
 */
Test(ServerHandlerClientPassword, TestPassIncorrectPassword)
{
	Client* client = mockClient();
	client->set_is_authenticated(false);

	Server server;
	server._password = "Password";
	server._clients.push_back(client);
	server._handler_client_password("IncorrectPassword", client->get_fd());

	cr_assert_eq(server._reply_code, 464);
}

/*
 * 4. O comando PASS é recebido e a senha do cliente está correta.
 */
Test(ServerHandlerClientPassword, TestPassCorrectPassword)
{
	Client* client = mockClient();
	client->set_is_authenticated(false);

	Server server;
	server._password = "Password";
	server._clients.push_back(client);
	server._handler_client_password("Password", client->get_fd());

	cr_assert_eq(server._reply_code, 200);
}