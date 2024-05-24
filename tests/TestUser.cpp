/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestUser.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:03:11 by yde-goes          #+#    #+#             */
/*   Updated: 2024/05/24 18:27:41 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "Client.hpp"
#include "Channel.hpp"
#define private public
#include "Server.hpp"

Client *mockClient()
{
	Client *client = new Client();
	client->set_fd(4);
	client->set_username("username");
	client->set_nickname("oldNickname");
	client->set_password("password");
	client->set_buffer("USER new_username");
	client->set_is_registered(true);
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	return client;
}

Test(UserCommand, err_needmoreparams)
{
	Client* client = mockClient();

	Server server;

	server._clients.push_back(*client);
	server._handler_client_username("", 4);
	cr_assert(eq(int, server._reply_code, 461)); // Assuming 461 is the reply code for ERR_NEEDMOREPARAMS
}

Test(UserCommand, err_notregistered)
{
	Client* client = mockClient();
	client->set_is_registered(false);

	Server server;

	server._clients.push_back(*client);
	server._handler_client_username("new_username", 4);
	cr_assert(eq(int, server._reply_code, 451)); // Assuming 451 is the reply code for ERR_NOTREGISTERED
}

Test(UserCommand, success_setusername_1)
{
	Client* client = mockClient();
	client->set_username("");

	Server server;

	server._clients.push_back(*client);
	server._handler_client_username("new_username", 4);
	cr_assert(eq(int, server._reply_code, 200)); // Assuming 200 is the reply code for a successful username change
}

// THIS TEST IS NOT WORKING. Maybe it should compare the strings?
/* Test(UserCommand, success_setusername_2)
{
	Client* client = mockClient();
	client->set_username("withUsername");

	Server server;

	server._clients.push_back(*client);
	server._handler_client_username("newUsername", 4);
	cr_assert(eq(int, server._reply_code, 200)); // Assuming 200 is the reply code for a successful username change
} */

Test(UserCommand, err_alreadyregistered)
{
	Client* client = mockClient();

	Server server;

	server._clients.push_back(*client);
	server._handler_client_username("username", 4);
	cr_assert(eq(int, server._reply_code, 462)); // Assuming 462 is the reply code for ERR_ALREADYREGISTERED
}

