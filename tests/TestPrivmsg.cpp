/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestPrivmsg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 09:03:10 by yde-goes          #+#    #+#             */
/*   Updated: 2024/06/01 11:19:37 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "Channel.hpp"
#include "Client.hpp"
#define private public
#include "Server.hpp"

/**
 * @brief Create a second receiver client for testing.
 *
 * @return Client* The second receiver client.
 */
Client* secondReceiver()
{
	Client* client = new Client();
	client->set_fd(6);
	client->set_username("secondUser");
	client->set_nickname("secondUser");
	client->set_password("password");
	client->set_buffer("");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	return client;
}

/**
 * @brief Create a first receiver client for testing.
 *
 * @return Client* The first receiver client.
 */
Client* firstReceiver()
{
	Client* client = new Client();
	client->set_fd(5);
	client->set_username("firstUser");
	client->set_nickname("firstUser");
	client->set_password("password");
	client->set_buffer("");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	return client;
}

/**
 * @brief Create a sender client for testing.
 *
 * @return Client* The sender client.
 */
Client* senderClient()
{
	Client* client = new Client();
	client->set_fd(4);
	client->set_username("senderUser");
	client->set_nickname("senderUser");
	client->set_password("password");
	client->set_buffer("");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	client->set_is_operator(false);
	return client;
}

/**
 * @brief Test case for successful PRIVMSG command to a single client.
 */
Test(PrivmsgCommand, privmsg_successfully_single_client)
{
	Client* sender = senderClient();
	Client* first = firstReceiver();
	Client* second = secondReceiver();

	Server server;

	server._clients.push_back(sender);
	server._clients.push_back(first);
	server._clients.push_back(second);

	server._handler_client_privmsg("firstUser :Hello, firstUser!", 4);
	cr_assert(eq(int, server._reply_code, 0));
}

/**
 * @brief Test case for successful PRIVMSG command to two clients.
 */
Test(PrivmsgCommand, privmsg_successfully_two_clients)
{
	Client* sender = senderClient();
	Client* first = firstReceiver();
	Client* second = secondReceiver();

	Server server;

	server._clients.push_back(sender);
	server._clients.push_back(first);
	server._clients.push_back(second);

	server._handler_client_privmsg("firstUser,secondUser :Hello, my Users!", 4);
	cr_assert(eq(int, server._reply_code, 0));
}

/**
 * @brief Test case for successful PRIVMSG command to a single channel.
 */
Test(PrivmsgCommand, privmsg_successfully_single_channel)
{
	Client* sender = senderClient();
	Client* first = firstReceiver();
	Client* second = secondReceiver();

	Server server;

	server._clients.push_back(sender);
	server._clients.push_back(first);
	server._clients.push_back(second);

	Channel* channel = new Channel("#brazil");

	channel->invite(sender);
	server._channels.push_back(channel);

	server._handler_client_privmsg("#brazil :Hello, BRAZIL!", 4);
	cr_assert(eq(int, server._reply_code, 0));
}

/**
 * @brief Test case for successful PRIVMSG command to three channels.
 */
Test(PrivmsgCommand, privmsg_successfully_three_channels)
{
	Client* sender = senderClient();
	Client* first = firstReceiver();
	Client* second = secondReceiver();

	Server server;

	server._clients.push_back(sender);
	server._clients.push_back(first);
	server._clients.push_back(second);

	Channel* ch_brazil = new Channel("#brazil");
	Channel* ch_france = new Channel("#france");
	Channel* ch_spain = new Channel("#spain");

	ch_brazil->invite(sender);
	ch_france->invite(sender);
	ch_france->invite(first);
	ch_france->invite(second);
	ch_spain->invite(sender);
	ch_spain->invite(first);
	server._channels.push_back(ch_brazil);
	server._channels.push_back(ch_france);
	server._channels.push_back(ch_spain);

	server._handler_client_privmsg("#brazil,#france,#spain :Hello, WORLD!", 4);
	cr_assert(eq(int, server._reply_code, 0));
}

/**
 * @brief Test case for successful PRIVMSG command to a mixed channel and user.
 */
Test(PrivmsgCommand, privmsg_successfully_mixed_channel_user)
{
	Client* sender = senderClient();
	Client* first = firstReceiver();
	Client* second = secondReceiver();

	Server server;

	server._clients.push_back(sender);
	server._clients.push_back(first);
	server._clients.push_back(second);

	Channel* ch_brazil = new Channel("#brazil");
	Channel* ch_france = new Channel("#france");
	Channel* ch_spain = new Channel("#spain");

	ch_brazil->invite(sender);
	ch_france->invite(sender);
	ch_spain->invite(sender);
	server._channels.push_back(ch_brazil);
	server._channels.push_back(ch_france);
	server._channels.push_back(ch_spain);

	server._handler_client_privmsg(
		"#brazil,firstUser,#france,secondUser,#spain :Hello, WORLD!", 4);
	cr_assert(eq(int, server._reply_code, 0));
}

/**
 * @brief Test case for ERR_NOTREGISTERED reply code.
 */
Test(PrivmsgCommand, err_notregistered)
{
	Client* sender = senderClient();
	Client* first = firstReceiver();
	Client* second = secondReceiver();

	Server server;

	sender->set_is_logged(false);

	server._clients.push_back(sender);
	server._clients.push_back(first);
	server._clients.push_back(second);

	server._handler_client_privmsg("firstUser :Hello, firstUser!", 4);
	cr_assert(eq(int, server._reply_code, 451));
}

/**
 * @brief Test case for ERR_NOSUCHCHANNEL reply code.
 */
Test(PrivmsgCommand, err_nosuchchannel)
{
	Client* sender = senderClient();
	Client* first = firstReceiver();
	Client* second = secondReceiver();

	Server server;

	server._clients.push_back(sender);
	server._clients.push_back(first);
	server._clients.push_back(second);

	Channel* ch_brazil = new Channel("#brazil");
	Channel* ch_france = new Channel("#france");
	Channel* ch_spain = new Channel("#spain");

	ch_brazil->invite(sender);
	ch_france->invite(sender);
	ch_spain->invite(sender);
	server._channels.push_back(ch_brazil);
	server._channels.push_back(ch_france);
	server._channels.push_back(ch_spain);

	server._handler_client_privmsg(
		"#brazil,firstUser,#neptune,secondUser,#spain :Hello, WORLD!", 4);
	cr_assert(eq(int, server._reply_code, 403));
}

/**
 * @brief Test case for ERR_NOTONCHANNEL reply code.
 */
Test(PrivmsgCommand, err_notonchannel)
{
	Client* sender = senderClient();
	Client* first = firstReceiver();
	Client* second = secondReceiver();

	Server server;

	server._clients.push_back(sender);
	server._clients.push_back(first);
	server._clients.push_back(second);

	Channel* ch_brazil = new Channel("#brazil");
	Channel* ch_france = new Channel("#france");
	Channel* ch_spain = new Channel("#spain");

	ch_brazil->invite(sender);
	ch_spain->invite(sender);
	server._channels.push_back(ch_brazil);
	server._channels.push_back(ch_france);
	server._channels.push_back(ch_spain);

	server._handler_client_privmsg(
		"#brazil,firstUser,#france,secondUser,#spain :Hello, WORLD!", 4);
	cr_assert(eq(int, server._reply_code, 442));
}

/**
 * @brief Test case for ERR_NOSUCHNICK reply code.
 */
Test(PrivmsgCommand, err_nosuchnick)
{
	Client* sender = senderClient();
	Client* first = firstReceiver();
	Client* second = secondReceiver();

	Server server;

	server._clients.push_back(sender);
	server._clients.push_back(first);
	server._clients.push_back(second);

	Channel* ch_brazil = new Channel("#brazil");
	Channel* ch_france = new Channel("#france");
	Channel* ch_spain = new Channel("#spain");

	ch_brazil->invite(sender);
	ch_france->invite(sender);
	ch_spain->invite(sender);
	server._channels.push_back(ch_brazil);
	server._channels.push_back(ch_france);
	server._channels.push_back(ch_spain);

	server._handler_client_privmsg(
		"#brazil,thirdUser,#france,secondUser,#spain :Hello, WORLD!", 4);
	cr_assert(eq(int, server._reply_code, 401));
}
