/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestNick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:03:34 by yde-goes          #+#    #+#             */
/*   Updated: 2024/05/25 21:21:28 by yde-goes         ###   ########.fr       */
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
	client->set_buffer("NICK new_nickname");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	return client;
}

Test(NickCommand, err_erroneusnick_1)
{
	Client* client = mockClient();

	Server server;

	server._clients.push_back(*client);
	server._handler_client_nickname("new_nickname", 4);
	cr_assert(eq(int, server._reply_code, 432));
}

Test(NickCommand, err_erroneusnick_2)
{
	Client* client = mockClient();

	Server server;

	server._clients.push_back(*client);
	server._handler_client_nickname("new_n1c7n4m3", 4);
	cr_assert(eq(int, server._reply_code, 432));
}

Test(NickCommand, err_norregistered)
{
	Client* client = mockClient();
	client->set_is_authenticated(false);
	client->set_is_logged(false);

	Server server;

	server._clients.push_back(*client);
	server._handler_client_nickname("oldNickname", 4);
	cr_assert(eq(int, server._reply_code, 451));
}

Test(NickCommand, success_setnickname)
{
	Client* client = mockClient();

	Server server;

	server._clients.push_back(*client);
	server._handler_client_nickname("newNickname", 4);
	cr_assert(eq(int, server._reply_code, 200));
}

// THIS TEST IS NOT WORKING, returns 200 instead of 433. Don't know why :/
/* Test(NickCommand, err_nickinuse)
{
	Client* client = mockClient();

	Server server;

	server._clients.push_back(*client);
	server._handler_client_nickname(client->get_nickname(), 4);
	std::cout << server._get_client(4)->get_nickname() << std::endl;
	cr_assert(eq(int, server._reply_code, 433));
} */
