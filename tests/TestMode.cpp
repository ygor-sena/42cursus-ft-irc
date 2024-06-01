/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestMode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 16:51:31 by gilmar            #+#    #+#             */
/*   Updated: 2024/06/01 11:13:44 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#define private public
#include "Server.hpp"

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

Channel* mockChannel()
{
	Channel* channel = new Channel("#channel");
	channel->set_topic("Channel Topic");
	channel->set_limit(10);
	channel->set_key("channelKey");
	return channel;
}

/**
 * 1. The MODE command is received without enough parameters.
 */
Test(ModeCommand, err_needmoreparams)
{
	Client* client = mockClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_mode("#channel", client->get_fd());

	cr_assert(eq(int, server._reply_code, 461));
}

/**
 * 2. The MODE command is received and the channel does not exist.
 */
Test(ModeCommand, err_nosuchchannel)
{
	Client* client = mockClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_mode("#channel +i", client->get_fd());

	cr_assert(eq(int, server._reply_code, 403));
}

/**
 * 3. The MODE command is received and the client does not have operator privileges in the channel.
 */
Test(ModeCommand, err_chanoprivsneeded)
{
	Client* client = mockClient();
	Channel* channel = mockChannel();

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_mode("#channel +i", client->get_fd());

	cr_assert(eq(int, server._reply_code, 482));
}

/**
 * 4. The MODE command is received and the channel mode is invalid.
 */
Test(ModeCommand, err_umodeunknownflag)
{
	Client* client = mockClient();

	Channel* channel = mockChannel();
	channel->set_channel_operator(client);

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_mode("#channel +z", client->get_fd());

	cr_assert(eq(int, server._reply_code, 472));
}

/**
 * 5. The MODE command is received and the channel mode is valid.
 */
Test(ModeCommand, mode_valid)
{
	Client* client = mockClient();

	Channel* channel = mockChannel();
	channel->set_channel_operator(client);

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_mode("#channel +i", client->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, channel->_invite_only, true));
}

/**
 * 6. The MODE command is received and the channel mode is successfully changed.
 */
Test(ModeCommand, mode_changed)
{
	Client* client = mockClient();

	Channel* channel = mockChannel();
	channel->set_channel_operator(client);

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_mode("#channel +i", client->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, channel->_invite_only, true));
}

/**
 * 7. The MODE command is received and the channel mode +i is successfully changed.
 */
Test(ModeCommand, mode_i_changed)
{
	Client* client = mockClient();

	Channel* channel = mockChannel();
	channel->set_channel_operator(client);

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_mode("#channel +i", client->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, channel->_invite_only, true));
}

/**
 * 8. The MODE command is received and the channel mode -i is successfully changed.
 */
Test(ModeCommand, mode_i_removed)
{
	Client* client = mockClient();

	Channel* channel = mockChannel();
	channel->set_channel_operator(client);
	channel->set_invite_only();

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_mode("#channel -i", client->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, channel->_invite_only, false));
}

/**
 * 9. The MODE command is received and the channel mode +t is successfully changed.
 */
Test(ModeCommand, mode_t_changed)
{
	Client* client = mockClient();

	Channel* channel = mockChannel();
	channel->set_channel_operator(client);

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_mode("#channel +t", client->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, channel->_topic_restriction, true));
}

/**
 * 10. The MODE command is received and the channel mode -t is successfully changed.
 */
Test(ModeCommand, mode_t_removed)
{
	Client* client = mockClient();

	Channel* channel = mockChannel();
	channel->set_channel_operator(client);
	channel->set_topic_restriction();

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_mode("#channel -t", client->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, channel->_topic_restriction, false));
}

/**
 * 11. The MODE command is received and the channel mode +k is successfully changed.
 */
Test(ModeCommand, mode_k_changed)
{
	Client* client = mockClient();

	Channel* channel = mockChannel();
	channel->set_channel_operator(client);

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_mode("#channel +k TesteChannelKey",
								client->get_fd());

	std::cout << "Channel Key: " << channel->_key << std::endl;

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, (channel->_key == "TesteChannelKey"), true));
}

/**
 * 12. The MODE command is received and the channel mode -k is successfully changed.
 */
Test(ModeCommand, mode_k_removed)
{
	Client* client = mockClient();

	Channel* channel = mockChannel();
	channel->set_channel_operator(client);
	channel->set_key("TesteChannelKey");

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_mode("#channel -k", client->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, channel->_key.empty(), true));
}

/**
 * 13. The MODE command is received and the channel mode +l is successfully changed.
 */
Test(ModeCommand, mode_l_changed)
{
	Client* client = mockClient();

	Channel* channel = mockChannel();
	channel->set_channel_operator(client);

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_mode("#channel +l 5", client->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, channel->_limit, 5));
}

/**
 * 14. The MODE command is received and the channel mode -l is successfully changed.
 */
Test(ModeCommand, mode_l_removed)
{
	Client* client = mockClient();

	Channel* channel = mockChannel();
	channel->set_channel_operator(client);
	channel->set_limit(5);

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_mode("#channel -l", client->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, channel->_limit, -1));
}

/**
 * 15. The MODE command is received and the channel mode +o is successfully changed.
 */
Test(ModeCommand, mode_o_changed)
{
	Client* client = mockClient();

	Channel* channel = mockChannel();
	channel->set_channel_operator(client);

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_mode("#channel +o trollUser", client->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, channel->is_channel_operator("trollUser"), true));
}

/**
 * 16. The MODE command is received and the channel mode -o is successfully changed.
 */
Test(ModeCommand, mode_o_removed)
{
	Client* client = mockClient();

	Channel* channel = mockChannel();
	channel->set_channel_operator(client);

	Server server;
	server._clients.push_back(client);
	server._channels.push_back(channel);
	server._handler_client_mode("#channel -o trollUser", client->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, channel->is_channel_operator("trollUser"), false));
}
