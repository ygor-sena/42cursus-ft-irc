/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestMarvinBot.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 18:23:35 by yde-goes          #+#    #+#             */
/*   Updated: 2024/06/01 11:12:00 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#define private public
#include "MarvinBot.hpp"

/**
 * @brief Mock function for creating a client.
 *
 * @return A pointer to the created client.
 */
Client* mockClient()
{
	Client* client = new Client();
	client->set_fd(5);
	client->set_username("trollUser");
	client->set_nickname("trollUser");
	client->set_password("password");
	client->set_buffer("");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	return client;
}

/**
 * @brief Test case for MarvinBotCommand when user is not in any channel.
 */
Test(MarvinBotCommand, user_is_not_in_any_channel)
{
	Client* client = mockClient();

	Server server;

	server._clients.push_back(client);

	server._handler_bot_marvin("!marvin", client->get_fd());

	cr_assert(eq(int, server._reply_code, 2424));
}

/**
 * @brief Test case for MarvinBotCommand when user is in any channel.
 */
Test(MarvinBotCommand, user_is_in_any_channel)
{
	Client* client = mockClient();

	Server server;
	Channel* channel = new Channel("#world");

	server._clients.push_back(client);

	channel->join(client);

	server._channels.push_back(channel);

	server._handler_bot_marvin("!marvin", client->get_fd());

	cr_assert(eq(int, server._reply_code, 4242));
}
