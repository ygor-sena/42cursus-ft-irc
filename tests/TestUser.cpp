/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestUser.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:03:11 by yde-goes          #+#    #+#             */
/*   Updated: 2024/06/01 11:27:02 by caalbert         ###   ########.fr       */
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
	/**
	 * @brief Create a mock client for testing.
	 *
	 * @return Pointer to the created client.
	 */
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

/**
 * @brief Test case for receiving the USER command with insufficient parameters.
 */
Test(UserCommand, err_needmoreparams)
{
	Client* client = mockClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_username("", client->get_fd());

	cr_assert(eq(int, server._reply_code, 461));
}

/**
 * @brief Test case for receiving the USER command when the client is not registered.
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

/**
 * @brief Test case for receiving the USER command when the client does not exist.
 */
Test(UserCommand, err_notexist)
{
	Server server;
	server._handler_client_username("Ygor", 5);

	cr_assert(eq(int, server._reply_code, 451));
}

/**
 * @brief Test case for receiving the USER command when the client is already registered.
 */
Test(UserCommand, err_alreadyregistered)
{
	Client* client = mockClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_username(client->get_username(), client->get_fd());

	cr_assert(eq(int, server._reply_code, 462));
}

/**
 * @brief Test case for receiving the USER command when the client is ready to log in.
 */
Test(UserCommand, rpl_readytologin)
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

/**
 * @brief Test case for receiving the USER command when the client is not ready to log in.
 */
Test(UserCommand, rpl_notreadytologin)
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
