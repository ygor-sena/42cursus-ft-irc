/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestNick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:03:34 by yde-goes          #+#    #+#             */
/*   Updated: 2024/06/01 15:56:11 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "Channel.hpp"
#include "Client.hpp"
#define private public
#include "Server.hpp"

/**
 * @brief Creates a mock client with predefined values.
 *
 * @return A pointer to the created client.
 */
Client* mockClient()
{
	Client* client = new Client();
	client->set_fd(4);
	client->set_username("Username");
	client->set_nickname("Nickname");
	client->set_password("Password");
	client->set_buffer("NICK Username");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	return client;
}

/**
 * @brief Test case for receiving the NICK command without enough parameters.
 */
Test(NickCommand, err_needmoreparams)
{
	Client* client = mockClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_nickname("", client->get_fd());

	cr_assert(eq(int, server._reply_code, 461));
}

/**
 * @brief Test case for receiving the NICK command when the client is not registered.
 */
Test(NickCommand, err_notregistered)
{
	Client* client = mockClient();
	client->set_is_authenticated(false);

	Server server;
	server._clients.push_back(client);
	server._handler_client_nickname(client->get_nickname(), client->get_fd());

	cr_assert(eq(int, server._reply_code, 451));
}

/**
 * @brief Test case for receiving the NICK command with an invalid client nickname.
 */
Test(NickCommand, err_erroneusnick_1)
{
	Client* client = mockClient();
	client->set_is_authenticated(true);

	Server server;
	server._clients.push_back(client);
	server._handler_client_nickname("new_nickname", client->get_fd());

	cr_assert(eq(int, server._reply_code, 432));
}

Test(NickCommand, err_erroneusnick_2)
{
	Client* client = mockClient();
	client->set_is_authenticated(true);

	Server server;
	server._clients.push_back(client);
	server._handler_client_nickname("$Gilmar", client->get_fd());

	cr_assert(eq(int, server._reply_code, 432));
}

Test(NickCommand, err_erroneusnick_3)
{
	Client* client = mockClient();
	client->set_is_authenticated(true);

	Server server;
	server._clients.push_back(client);
	server._handler_client_nickname("Ygor%", client->get_fd());

	cr_assert(eq(int, server._reply_code, 432));
}

Test(NickCommand, err_erroneusnick_4)
{
	Client* client = mockClient();
	client->set_is_authenticated(true);

	Server server;
	server._clients.push_back(client);
	server._handler_client_nickname("Car#los", client->get_fd());

	cr_assert(eq(int, server._reply_code, 432));
}

/**
 * @brief Test case for receiving the NICK command when the client is registered.
 */
Test(NickCommand, success_setnickname)
{
	Client* client = mockClient();
	client->set_is_authenticated(true);

	Server server;
	server._clients.push_back(client);
	server._handler_client_nickname("newNickname", client->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
}

/**
 * @brief Test case for receiving the NICK command with a nickname that is already in use.
 */
Test(NickCommand, err_nickinuse)
{
	Client* client = mockClient();
	client->set_is_authenticated(true);

	Client* client1 = mockClient();
	client1->set_fd(5);
	client1->set_is_authenticated(true);

	Server server;
	server._clients.push_back(client);
	server._clients.push_back(client1);
	server._handler_client_nickname(client1->get_nickname(), client1->get_fd());

	cr_assert(eq(int, server._reply_code, 433));
}

/**
 * @brief Test case for receiving the NICK command and changing the client's nickname.
 */
Test(NickCommand, success_changenickname)
{
	Client* client = mockClient();
	client->set_is_authenticated(true);

	Server server;
	server._clients.push_back(client);
	server._handler_client_nickname(client->get_nickname(), client->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
}

/**
 * @brief Test case for receiving the NICK command when the client is ready to log in.
 */
Test(NickCommand, success_readytologin)
{
	Client* client = mockClient();
	client->set_is_authenticated(true);
	client->set_is_logged(false);

	Server server;
	server._clients.push_back(client);
	server._handler_client_nickname(client->get_nickname(), client->get_fd());

	cr_assert(eq(int, server._reply_code, 001));
}
