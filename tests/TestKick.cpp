/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestKick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:15:38 by yde-goes          #+#    #+#             */
/*   Updated: 2024/06/01 11:11:02 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "Channel.hpp"
#include "Client.hpp"
#define private public
#include "Server.hpp"

Client* mockOutsideClient()
{
	Client* client = new Client();
	client->set_fd(6);
	client->set_username("outsideUser");
	client->set_nickname("outsideUser");
	client->set_password("password");
	client->set_buffer("");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	return client;
}

Client* mockCommonClient()
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

Client* mockOperatorClient()
{
	Client* client = new Client();
	client->set_fd(4);
	client->set_username("channelOperator");
	client->set_nickname("channelOperator");
	client->set_password("password");
	client->set_buffer("");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	client->set_is_operator(false);
	return client;
}

/**
 * @brief Test case for successful kick command without comments.
 */
Test(KickCommand, kick_successfully_no_comments)
{
	Client* troll = mockCommonClient();
	Client* channelOperator = mockOperatorClient();

	Server server;
	Channel* channel = new Channel("#world");

	server._clients.push_back(troll);
	server._clients.push_back(channelOperator);

	channel->invite(troll);
	channel->invite(channelOperator);

	channelOperator->set_is_operator(true);
	channel->set_channel_operator(channelOperator);

	server._channels.push_back(channel);
	server._handler_client_kick("#world trollUser", 4);

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(
		int, server._get_channel(channel->get_name())->get_clients_size(), 1));
}

/**
 * @brief Test case for successful kick command with comments.
 */
Test(KickCommand, kick_successfully_with_comments)
{
	Client* troll = mockCommonClient();
	Client* channelOperator = mockOperatorClient();

	Server server;
	Channel* channel = new Channel("#world");

	server._clients.push_back(troll);
	server._clients.push_back(channelOperator);

	channel->invite(troll);
	channel->invite(channelOperator);

	channelOperator->set_is_operator(true);
	channel->set_channel_operator(channelOperator);

	server._channels.push_back(channel);
	server._handler_client_kick("#world trollUser trolou", 4);

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(
		int, server._get_channel(channel->get_name())->get_clients_size(), 1));
}

/**
 * @brief Test case for error when channel does not exist.
 */
Test(KickCommand, err_nosuchchannel)
{
	Client* troll = mockCommonClient();
	Client* channelOperator = mockOperatorClient();

	Server server;
	Channel* channel = new Channel("#world");

	server._clients.push_back(troll);
	server._clients.push_back(channelOperator);

	channel->invite(troll);
	channel->invite(channelOperator);

	channel->set_channel_operator(channelOperator);

	server._channels.push_back(channel);

	server._handler_client_kick("#deadworld trollUser", 4);
	cr_assert(eq(int, server._reply_code, 403));
}

/**
 * @brief Test case for error when client is not in the channel.
 */
Test(KickCommand, err_notonchannel)
{
	Client* troll = mockCommonClient();
	Client* channelOperator = mockOperatorClient();
	Client* outsideUser = mockOutsideClient();

	Server server;
	Channel* channel = new Channel("#world");

	server._clients.push_back(troll);
	server._clients.push_back(channelOperator);
	server._clients.push_back(outsideUser);

	channel->invite(troll);
	channel->invite(channelOperator);

	channel->set_channel_operator(channelOperator);

	server._channels.push_back(channel);

	server._handler_client_kick("#world trollUser", 6);
	cr_assert(eq(int, server._reply_code, 442));
}

/**
 * @brief Test case for error when client is not a channel operator.
 */
Test(KickCommand, err_chanoprivsneeded)
{
	Client* troll = mockCommonClient();
	Client* channelOperator = mockOperatorClient();

	Server server;
	Channel* channel = new Channel("#world");

	channelOperator->set_is_operator(false);

	server._clients.push_back(troll);
	server._clients.push_back(channelOperator);

	channel->invite(troll);
	channel->invite(channelOperator);

	server._channels.push_back(channel);

	server._handler_client_kick("#world trollUser", 4);
	cr_assert(eq(int, server._reply_code, 482));
}

/**
 * @brief Test case for error when client nickname does not exist.
 */
Test(KickCommand, err_nosuchnick)
{
	Client* troll = mockCommonClient();
	Client* channelOperator = mockOperatorClient();

	Server server;
	Channel* channel = new Channel("#world");

	channelOperator->set_is_operator(false);

	server._clients.push_back(troll);
	server._clients.push_back(channelOperator);

	channel->invite(troll);
	channel->invite(channelOperator);

	channelOperator->set_is_operator(true);
	channel->set_channel_operator(channelOperator);

	server._channels.push_back(channel);

	server._handler_client_kick("#world noUserInServer", 4);
	cr_assert(eq(int, server._reply_code, 401));
}

/**
 * @brief Test case for error when client is not in the channel.
 */
Test(KickCommand, err_usernotinchannel)
{
	Client* troll = mockCommonClient();
	Client* channelOperator = mockOperatorClient();
	Client* outsideUser = mockOutsideClient();

	Server server;
	Channel* channel = new Channel("#world");

	channelOperator->set_is_operator(false);

	server._clients.push_back(troll);
	server._clients.push_back(channelOperator);
	server._clients.push_back(outsideUser);

	channel->invite(troll);
	channel->invite(channelOperator);

	channelOperator->set_is_operator(true);
	channel->set_channel_operator(channelOperator);

	server._channels.push_back(channel);

	server._handler_client_kick("#world outsideUser", 4);
	cr_assert(eq(int, server._reply_code, 441));
}

