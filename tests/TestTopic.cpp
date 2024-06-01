/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestTopic.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 08:57:00 by yde-goes          #+#    #+#             */
/*   Updated: 2024/06/01 16:03:05 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "Channel.hpp"
#include "Client.hpp"
#define private public
#include "Server.hpp"

Client* mockChannelMemberClient()
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
 * @brief Test case for receiving the TOPIC command with insufficient parameters.
 */
Test(TopicCommand, err_needmoreparams)
{
	Client* client = mockChannelMemberClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_topic("", client->get_fd());

	cr_assert(eq(int, server._reply_code, 461));
}

/**
 * @brief Test case for receiving the TOPIC command when the client is not logged in.
 */
Test(TopicCommand, err_notregistered)
{
	Client* client = mockChannelMemberClient();
	client->set_is_logged(false);

	Server server;
	server._clients.push_back(client);
	server._handler_client_topic("#IRC", client->get_fd());

	cr_assert(eq(int, server._reply_code, 451));
}

/**
 * @brief Test case for receiving the TOPIC command when the channel does not exist.
 */
Test(TopicCommand, err_nosuchchannel)
{
	Client* client = mockChannelMemberClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_topic("#channel", client->get_fd());

	cr_assert(eq(int, server._reply_code, 403));
}

/**
 * @brief Test case for receiving the TOPIC command when the client is not in the channel.
 */
Test(TopicCommand, err_notonchannel)
{
	Client* client = mockChannelMemberClient();

	Channel* channel = new Channel("#channel");

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_topic("#channel", client->get_fd());

	cr_assert(eq(int, server._reply_code, 442));
}

/**
 * @brief Test case for receiving the TOPIC command when the channel does not have a topic.
 */
Test(TopicCommand, rpl_notopic)
{
	Client* client = mockChannelMemberClient();

	Channel* channel = new Channel("#channel");
	channel->join(client);

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);

	server._handler_client_topic("#channel", client->get_fd());
	cr_assert(eq(int, server._reply_code, 331));
}

/**
 * @brief Test case for receiving the TOPIC command when the channel has a topic.
 */
Test(TopicCommand, rpl_topic)
{
	Client* client = mockChannelMemberClient();

	Channel* channel = new Channel("#channel");
	channel->join(client);
	channel->set_topic("Hello World!");

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_topic("#channel", client->get_fd());

	cr_assert(eq(int, server._reply_code, 332));
}

/**
 * @brief Test case for receiving the TOPIC command when the client is not an operator of the channel.
 */
Test(TopicCommand, err_chanoprivsneeded)
{
	Client* client = mockChannelMemberClient();

	Channel* channel = new Channel("#channel");
	channel->join(client);

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_topic("#channel newtopic", client->get_fd());

	cr_assert(eq(int, server._reply_code, 482));
}

/**
 * @brief Test case for receiving the TOPIC command when the channel has topic restriction.
 */
Test(TopicCommand, err_chanoprivsneeded_topic_restriction)
{
	Client* client = mockChannelMemberClient();

	Channel* channel = new Channel("#channel");
	channel->join(client);
	channel->set_channel_operator(client);
	channel->set_topic_restriction();

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_topic("#channel newtopic", client->get_fd());

	cr_assert(eq(int, server._reply_code, 482));
}

/**
 * @brief Test case for receiving the TOPIC command and successfully changing the topic.
 */
Test(TopicCommand, rpl_topic_set)
{
	Client* client = mockChannelMemberClient();

	Channel* channel = new Channel("#channel");
	channel->join(client);
	channel->set_channel_operator(client);

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_topic("#channel newtopic", client->get_fd());

	cr_assert(eq(int, server._reply_code, 332));
}
