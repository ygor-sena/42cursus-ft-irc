/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestPass.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:27:49 by gilmar            #+#    #+#             */
/*   Updated: 2024/06/01 09:01:18 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "Channel.hpp"
#include "Client.hpp"
#define private public
#include "Server.hpp"

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
Test(ServerHandlerClientPassword, err_needmoreparams)
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
Test(ServerHandlerClientPassword, err_alreadyregistered)
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
Test(ServerHandlerClientPassword, err_incorpass)
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
Test(ServerHandlerClientPassword, rpl_pass)
{
	Client* client = mockClient();
	client->set_is_authenticated(false);

	Server server;
	server._password = "Password";
	server._clients.push_back(client);
	server._handler_client_password("Password", client->get_fd());

	cr_assert_eq(server._reply_code, 200);
}
