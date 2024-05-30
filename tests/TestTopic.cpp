/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestTopic.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 08:57:00 by yde-goes          #+#    #+#             */
/*   Updated: 2024/05/30 15:58:11 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "Client.hpp"
#include "Channel.hpp"
#define private public
#include "Server.hpp"

/*
** TOPIC #channel
** - mostrar o tópico do canal se houver (332), caso contrário, o canal não tem tópico (331)
**
** TOPIC #channel newtopic
** - para alterar o tópico, o cliente deve ser operador do canal (482)
** - mode -t permite ou não alterar o tópico do canal
*/

Client *mockChannelMemberClient()
{
	Client *client = new Client();
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

Test(TopicCommand, err_needmoreparams)
{
	Client *client = mockChannelMemberClient();
	Server server;

	server._clients.push_back(*client);
	server._handler_client_topic("", client->get_fd());
	cr_assert(eq(int, server._reply_code, 403));
}

Test(TopicCommand, err_notregistered)
{
	Client *client = mockChannelMemberClient();
	Server server;

	client->set_is_logged(false);

	server._clients.push_back(*client);
	server._handler_client_topic("", client->get_fd());
	cr_assert(eq(int, server._reply_code, 451));
}

Test(TopicCommand, err_nosuchchannel)
{
	Client *client = mockChannelMemberClient();
	Server server;

	server._clients.push_back(*client);
	server._handler_client_topic("#channel", client->get_fd());
	cr_assert(eq(int, server._reply_code, 403));
}

Test(TopicCommand, rpl_notopic)
{
	Client *client = mockChannelMemberClient();
	Server server;

	Channel *channel = new Channel("#channel");

	server._clients.push_back(*client);

	// Add clients as channel member
	channel->join(client);

	// Client is NOT operator
	client->set_is_operator(false);

	// Add channel to the server channels list
	server._channels.push_back(channel);

	server._handler_client_topic("#channel", client->get_fd());
	cr_assert(eq(int, server._reply_code, 331));
}

Test(TopicCommand, err_notoperator)
{
	Client *client = mockChannelMemberClient();
	Server server;

	Channel *channel = new Channel("#channel");

	server._clients.push_back(*client);

	// Add clients as channel member
	channel->join(client);
	client->set_is_operator(false);

	// Add channel to the server channels list
	server._channels.push_back(channel);

	server._handler_client_topic("#channel newtopic", client->get_fd());
	cr_assert(eq(int, server._reply_code, 482));
}

Test(TopicCommand, rpl_topic_set_first_topic)
{
	Client *client = mockChannelMemberClient();
	Server server;

	Channel *channel = new Channel("#channel");

	server._clients.push_back(*client);

	// Add clients as channel member
	channel->join(client);
	channel->set_channel_operator(client);

	// Add channel to the server channels list
	server._channels.push_back(channel);

	server._handler_client_topic("#channel newtopic", client->get_fd());
	cr_assert(eq(int, server._reply_code, 332));
}

Test(TopicCommand, rpl_topic_set_second_topic)
{
	Client *client = mockChannelMemberClient();
	Server server;

	Channel *channel = new Channel("#channel");

	server._clients.push_back(*client);

	// Add clients as channel member
	channel->join(client);
	channel->set_channel_operator(client);

	// Add channel to the server channels list
	server._channels.push_back(channel);

	server._handler_client_topic("#channel newtopic", client->get_fd());
	cr_assert(eq(int, server._reply_code, 332));
	server._handler_client_topic("#channel secondtopic", client->get_fd());
	cr_assert(eq(int, server._reply_code, 332));
}

Test(TopicCommand, cannot_set_topic_with_topic_restriction)
{
	Client *client = mockChannelMemberClient();
	Server server;

	Channel *channel = new Channel("#channel");

	server._clients.push_back(*client);

	// Add clients as channel member
	channel->join(client);
	channel->set_channel_operator(client);
	channel->set_topic_restriction();

	// Add channel to the server channels list
	server._channels.push_back(channel);

	server._handler_client_topic("#channel newtopic", client->get_fd());
	cr_assert(eq(int, server._reply_code, 482));
}

Test(TopicCommand, err_notonchannel)
{
	Client *client = mockChannelMemberClient();
	Server server;

	Channel *channel = new Channel("#channel");

	server._clients.push_back(*client);

	// Add channel to the server channels list
	server._channels.push_back(channel);

	server._handler_client_topic("#channel newtopic", client->get_fd());
	cr_assert(eq(int, server._reply_code, 442));
}
