/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestJoin.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:15:35 by yde-goes          #+#    #+#             */
/*   Updated: 2024/06/01 15:40:58 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "Channel.hpp"
#include "Client.hpp"
#define private public
#include "Server.hpp"

/**
 * @brief Creates a mock client outside the server.
 *
 * @return A pointer to the created client.
 */
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

/**
 * @brief Creates a mock common client.
 *
 * @return A pointer to the created client.
 */
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

/**
 * @brief Creates a mock operator client.
 *
 * @return A pointer to the created client.
 */
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
 * @brief Test case for the JOIN command with insufficient parameters.
 */
Test(JoinCommand, err_needmoreparams)
{
	Client* client = mockCommonClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_join("", client->get_fd());

	cr_assert(eq(int, server._reply_code, 461));
}

/**
 * @brief Test case for the JOIN command when the client is not registered.
 */
Test(JoinCommand, err_notregistered)
{
	Client* client = mockOutsideClient();
	client->set_is_logged(false);

	Server server;
	server._clients.push_back(client);
	server._handler_client_join("#channel", client->get_fd());

	cr_assert(eq(int, server._reply_code, 451));
}

/**
 * @brief Test case for the JOIN command with an invalid channel mask.
 */
Test(JoinCommand, err_badchanmask)
{
	Client* client = mockCommonClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_join("channel", client->get_fd());

	cr_assert(eq(int, server._reply_code, 403));
}

/**
 * @brief Test case for the JOIN command when the channel does not exist.
 * The channel is created and the client is added as an operator.
 */
Test(JoinCommand, channel_created)
{
	Client* client = mockCommonClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_join("#channel", client->get_fd());

	cr_assert(eq(int, server._channels.size(), 1));
	cr_assert(eq(int, server._channels[0]->get_name() == "#channel", true));
	cr_assert(eq(int, server._channels[0]->has_client(client), true));
	cr_assert(eq(int, client->get_is_operator(), true));
	cr_assert(eq(int, server._reply_code, 200));
}

/**
 * @brief Test case for the JOIN command when the client is already in the channel.
 */
Test(JoinCommand, err_alreadyregistered)
{
	Client* client = mockCommonClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_join("#channel", client->get_fd());
	server._handler_client_join("#channel", client->get_fd());

	cr_assert(eq(int, server._reply_code, 462));
	cr_assert(eq(int, server._channels.size(), 1));
	cr_assert(eq(int, server._channels[0]->get_clients_size(), 1));
	cr_assert(eq(int, server._channels[0]->has_client(client), true));
}

/**
 * @brief Test case for the JOIN command when the channel is full.
 */
Test(JoinCommand, err_channelisfull)
{
	Client* client1 = mockCommonClient();
	Client* client2 = mockOutsideClient();
	Client* client3 = mockOperatorClient();

	Server server;
	server._clients.push_back(client1);
	server._clients.push_back(client2);
	server._clients.push_back(client3);

	server._handler_client_join("#channel", client3->get_fd());
	server._handler_client_mode("#channel +l 2", client3->get_fd());
	server._handler_client_join("#channel", client1->get_fd());
	server._handler_client_join("#channel", client2->get_fd());

	cr_assert(eq(int, server._reply_code, 422));
	cr_assert(eq(int, server._channels.size(), 1));
	cr_assert(eq(int, server._channels[0]->get_clients_size(), 2));
	cr_assert(eq(int, server._channels[0]->has_client(client1), true));
	cr_assert(eq(int, server._channels[0]->has_client(client3), true));
	cr_assert(eq(int, server._channels[0]->has_client(client2), false));
}

/**
 * @brief Test case for the JOIN command when the channel is invite-only.
 */
Test(JoinCommand, err_inviteonlychan)
{
	Client* client1 = mockCommonClient();
	Client* client2 = mockOutsideClient();
	Client* client3 = mockOperatorClient();

	Server server;
	server._clients.push_back(client1);
	server._clients.push_back(client2);
	server._clients.push_back(client3);

	server._handler_client_join("#channel", client3->get_fd());
	server._handler_client_mode("#channel +i", client3->get_fd());

	server._handler_client_join("#channel", client1->get_fd());
	server._handler_client_join("#channel", client2->get_fd());

	cr_assert(eq(int, server._reply_code, 473));
	cr_assert(eq(int, server._channels.size(), 1));
	cr_assert(eq(int, server._channels[0]->get_clients_size(), 1));
	cr_assert(eq(int, server._channels[0]->has_client(client3), true));
	cr_assert(eq(int, server._channels[0]->has_client(client1), false));
	cr_assert(eq(int, server._channels[0]->has_client(client2), false));
}

/**
 * @brief Test case for the JOIN command when the channel is invite-only.
 */
Test(JoinCommand, err_inviteonlychan_not_invited)
{
	Client* client1 = mockCommonClient();
	Client* client2 = mockOutsideClient();
	Client* client3 = mockOperatorClient();

	Server server;
	server._clients.push_back(client1);
	server._clients.push_back(client2);
	server._clients.push_back(client3);

	server._handler_client_join("#channel", client3->get_fd());
	server._handler_client_mode("#channel +i", client3->get_fd());

	server._handler_client_join("#channel", client2->get_fd());
	server._handler_client_join("#channel", client1->get_fd());

	cr_assert(eq(int, server._reply_code, 473));
	cr_assert(eq(int, server._channels.size(), 1));
	cr_assert(eq(int, server._channels[0]->get_clients_size(), 1));
	cr_assert(eq(int, server._channels[0]->has_client(client3), true));
	cr_assert(eq(int, server._channels[0]->has_client(client1), false));
	cr_assert(eq(int, server._channels[0]->has_client(client2), false));
}

/**
 * @brief Test case for the JOIN command when the client is not invited to the invite-only channel.
 */
Test(JoinCommand, inviteonlychan_invited)
{
	Client* client1 = mockCommonClient();
	Client* client2 = mockOutsideClient();
	Client* client3 = mockOperatorClient();

	Server server;
	server._clients.push_back(client1);
	server._clients.push_back(client2);
	server._clients.push_back(client3);

	server._handler_client_join("#channel", client3->get_fd());

	server._handler_client_mode("#channel +i", client3->get_fd());

	server._handler_client_invite("trollUser #channel", client3->get_fd());
	server._handler_client_invite("outsideUser #channel", client3->get_fd());

	server._handler_client_join("#channel", client1->get_fd());
	server._handler_client_join("#channel", client2->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, server._channels.size(), 1));
	cr_assert(eq(int, server._channels[0]->get_clients_size(), 3));
	cr_assert(eq(int, server._channels[0]->has_client(client3), true));
	cr_assert(eq(int, server._channels[0]->has_client(client1), true));
	cr_assert(eq(int, server._channels[0]->has_client(client2), true));
}

/**
 * @brief Test case for the JOIN command when the client is not invited to the invite-only channel.
 */
Test(JoinCommand, inviteonlychan_invited_with_key)
{
	Client* client1 = mockCommonClient();
	Client* client2 = mockOutsideClient();
	Client* client3 = mockOperatorClient();

	Server server;
	server._clients.push_back(client1);
	server._clients.push_back(client2);
	server._clients.push_back(client3);

	server._handler_client_join("#channel", client3->get_fd());

	server._handler_client_mode("#channel +i", client3->get_fd());
	server._handler_client_mode("#channel +k password", client3->get_fd());

	server._handler_client_invite("trollUser #channel", client3->get_fd());
	server._handler_client_invite("outsideUser #channel", client3->get_fd());

	server._handler_client_join("#channel password", client1->get_fd());
	server._handler_client_join("#channel password", client2->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, server._channels.size(), 1));
	cr_assert(eq(int, server._channels[0]->get_clients_size(), 3));
	cr_assert(eq(int, server._channels[0]->has_client(client3), true));
	cr_assert(eq(int, server._channels[0]->has_client(client1), true));
	cr_assert(eq(int, server._channels[0]->has_client(client2), true));
}

/**
 * @brief Test case for the JOIN command when the client is invited to the invite-only channel.
 */
Test(JoinCommand, err_invite_only_badchannelkey)
{
	Client* client1 = mockCommonClient();
	Client* client2 = mockOutsideClient();
	Client* client3 = mockOperatorClient();

	Server server;
	server._clients.push_back(client1);
	server._clients.push_back(client2);
	server._clients.push_back(client3);

	server._handler_client_join("#channel", client3->get_fd());

	server._handler_client_mode("#channel +i", client3->get_fd());
	server._handler_client_mode("#channel +k password", client3->get_fd());

	server._handler_client_invite("trollUser #channel", client3->get_fd());
	server._handler_client_invite("outsideUser #channel", client3->get_fd());

	server._handler_client_join("#channel", client1->get_fd());
	server._handler_client_join("#channel err_password", client2->get_fd());

	cr_assert(eq(int, server._reply_code, 475));
	cr_assert(eq(int, server._channels.size(), 1));
	cr_assert(eq(int, server._channels[0]->get_clients_size(), 1));
	cr_assert(eq(int, server._channels[0]->has_client(client3), true));
	cr_assert(eq(int, server._channels[0]->has_client(client1), false));
	cr_assert(eq(int, server._channels[0]->has_client(client2), false));
}

/**
 * @brief Test case for the JOIN command when the client enters the invite-only channel with the correct key.
 */
Test(JoinCommand, err_badchannelkey)
{
	Client* client1 = mockCommonClient();
	Client* client2 = mockOutsideClient();
	Client* client3 = mockOperatorClient();

	Server server;
	server._clients.push_back(client1);
	server._clients.push_back(client2);
	server._clients.push_back(client3);

	server._handler_client_join("#channel password", client3->get_fd());

	server._handler_client_mode("#channel +k password", client3->get_fd());

	server._handler_client_join("#channel", client1->get_fd());
	server._handler_client_join("#channel err_password", client2->get_fd());

	cr_assert(eq(int, server._reply_code, 475));
	cr_assert(eq(int, server._channels.size(), 1));
	cr_assert(eq(int, server._channels[0]->get_clients_size(), 1));
	cr_assert(eq(int, server._channels[0]->has_client(client3), true));
	cr_assert(eq(int, server._channels[0]->has_client(client1), false));
	cr_assert(eq(int, server._channels[0]->has_client(client2), false));
}

/**
 * @brief Test case for the JOIN command when the client enters the channel with a key.
 */
Test(JoinCommand, channel_with_key)
{
	Client* client1 = mockCommonClient();
	Client* client2 = mockOutsideClient();
	Client* client3 = mockOperatorClient();

	Server server;
	server._clients.push_back(client1);
	server._clients.push_back(client2);
	server._clients.push_back(client3);

	server._handler_client_join("#channel password", client3->get_fd());

	server._handler_client_mode("#channel +k password", client3->get_fd());

	server._handler_client_join("#channel password", client1->get_fd());
	server._handler_client_join("#channel password", client2->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, server._channels.size(), 1));
	cr_assert(eq(int, server._channels[0]->get_clients_size(), 3));
	cr_assert(eq(int, server._channels[0]->has_client(client3), true));
	cr_assert(eq(int, server._channels[0]->has_client(client1), true));
	cr_assert(eq(int, server._channels[0]->has_client(client2), true));
}

/**
 * @brief Test case for the JOIN command when the client enters the channel without a key.
 */
Test(JoinCommand, channel_without_key)
{
	Client* client1 = mockCommonClient();
	Client* client2 = mockOutsideClient();
	Client* client3 = mockOperatorClient();

	Server server;
	server._clients.push_back(client1);
	server._clients.push_back(client2);
	server._clients.push_back(client3);

	server._handler_client_join("#channel", client3->get_fd());
	server._handler_client_join("#channel", client1->get_fd());
	server._handler_client_join("#channel", client2->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, server._channels.size(), 1));
	cr_assert(eq(int, server._channels[0]->get_clients_size(), 3));
	cr_assert(eq(int, server._channels[0]->has_client(client3), true));
}
