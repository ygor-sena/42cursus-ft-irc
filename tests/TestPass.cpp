/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestPass.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:27:49 by gilmar            #+#    #+#             */
/*   Updated: 2024/06/01 15:58:47 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "Channel.hpp"
#include "Client.hpp"
#define private public
#include "Server.hpp"

/**
 * @brief Creates a mock client object with predefined values.
 * @return A pointer to the created client object.
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

/**
 * @brief Test case for handling the PASS command when received without enough
 * parameters.
 */
Test(ServerHandlerClientPassword, err_needmoreparams)
{
	Client* client = mockClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_password("", client->get_fd());

	cr_assert_eq(server._reply_code, 461);
}

/**
 * @brief Test case for handling the PASS command when received and the client
 * is already authenticated.
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

/**
 * @brief Test case for handling the PASS command when received and the client's
 * password is incorrect.
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

/**
 * @brief Test case for handling the PASS command when received and the client's
 * password is correct.
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
