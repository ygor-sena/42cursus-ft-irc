/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestPart.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 08:56:56 by yde-goes          #+#    #+#             */
/*   Updated: 2024/06/01 15:57:30 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "Channel.hpp"
#include "Client.hpp"
#define private public
#include "Server.hpp"

/**
 * @brief Creates a generic client for testing.
 * @return A pointer to the created client.
 */
Client* genericClient()
{
	Client* client = new Client();
	client->set_fd(6);
	client->set_username("soAndSoUser");
	client->set_nickname("soAndSoUser");
	client->set_password("password");
	client->set_buffer("");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	return client;
}

/**
 * @brief Creates a client for testing the PART command.
 * @return A pointer to the created client.
 */
Client* toPartClient()
{
	Client* client = new Client();
	client->set_fd(4);
	client->set_username("toPartUser");
	client->set_nickname("toPartUser");
	client->set_password("password");
	client->set_buffer("");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	client->set_is_operator(false);
	return client;
}

/**
 * @brief Tests the PART command when it is received without enough parameters.
 */
Test(PartCommand, part_without_enough_parameters)
{
	Client* client = genericClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_part("", client->get_fd());

	cr_assert(eq(int, server._reply_code, 461));
}

/**
 * @brief Tests the PART command when the client is not registered.
 */
Test(PartCommand, part_not_registered)
{
	Client* client = genericClient();
	client->set_is_logged(false);

	Server server;
	server._clients.push_back(client);
	server._handler_client_part("#channel", client->get_fd());

	cr_assert(eq(int, server._reply_code, 451));
}

/**
 * @brief Tests the PART command when the channel does not exist.
 */
Test(PartCommand, part_channel_does_not_exist)
{
	Client* client = genericClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_part("#nonExistent", client->get_fd());

	cr_assert(eq(int, server._reply_code, 403));
}

/**
 * @brief Tests the PART command when the client is not in the channel.
 */
Test(PartCommand, part_not_on_channel)
{
	Client* client = genericClient();

	Channel* channel = new Channel("#channel");

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_part("#channel", client->get_fd());

	cr_assert(eq(int, server._reply_code, 442));
}

/**
 * @brief Tests the PART command when the client successfully leaves the
 * channel.
 */
Test(PartCommand, part_successfully)
{
	Client* client = genericClient();

	Channel* channel = new Channel("#channel");
	channel->invite(client);

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_part("#channel", client->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, channel->get_clients_size(), 0));
}
