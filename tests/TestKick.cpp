/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestKick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:15:38 by yde-goes          #+#    #+#             */
/*   Updated: 2024/05/30 15:14:01 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "Client.hpp"
#include "Channel.hpp"
#define private public
#include "Server.hpp"

Client *mockOutsideClient()
{
	Client *client = new Client();
	client->set_fd(6);
	client->set_username("outsideUser");
	client->set_nickname("outsideUser");
	client->set_password("password");
	client->set_buffer("");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	return client;
}

Client *mockCommonClient()
{
	Client *client = new Client();
	client->set_fd(5);
	client->set_username("trollUser");
	client->set_nickname("trollUser");
	client->set_password("password");
	client->set_buffer("");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	return client;
}

Client *mockOperatorClient()
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

Test(KickCommand, kick_successfully_no_comments)
{
	Client* troll = mockCommonClient();
	Client* channelOperator = mockOperatorClient();

	Server server;
	Channel *channel = new Channel("#world");

	// Add clients to the server clients list
	server._clients.push_back(*troll);
	server._clients.push_back(*channelOperator);

	// Add clients as channel member
	channel->invite(troll);
	channel->invite(channelOperator);

	// Set client channelOperator as channel operator
	// WARNING: for some reason, the channel must be added
	// to the server channels list AFTER setting the operator
	channelOperator->set_is_operator(true);
	channel->set_channel_operator(channelOperator);
	
	// Add channel to the server channels list
	server._channels.push_back(channel);

	// Kick troll from the channel
	server._handler_client_kick("#world trollUser", 4);
	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, server._get_channel(channel->get_name())->get_clients_size(), 1));
}

Test(KickCommand, kick_successfully_with_comments)
{
	Client* troll = mockCommonClient();
	Client* channelOperator = mockOperatorClient();

	Server server;
	Channel *channel = new Channel("#world");

	// Add clients to the server clients list
	server._clients.push_back(*troll);
	server._clients.push_back(*channelOperator);

	// Add clients as channel member
	channel->invite(troll);
	channel->invite(channelOperator);

	// Set client channelOperator as channel operator
	// WARNING: for some reason, the channel must be added
	// to the server channels list AFTER setting the operator
	channelOperator->set_is_operator(true);
	channel->set_channel_operator(channelOperator);
	
	// Add channel to the server channels list
	server._channels.push_back(channel);

	// Print get_operator_clients
	/*std::vector<Client *> operator_clients = channel->get_operator_clients();

	for (std::vector<Client *>::const_iterator it = operator_clients.begin(); it != operator_clients.end(); ++it)
	{
		std::cout << (*it)->get_nickname() << std::endl;
		std::cout << (*it)->get_fd() << std::endl;
		std::cout << (*it)->get_is_operator() << std::endl;
	} */

	// Kick troll from the channel
	server._handler_client_kick("#world trollUser trolou", 4);
	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, server._get_channel(channel->get_name())->get_clients_size(), 1));
}

/* Test(KickCommand, err_needmoreparams)
{
	Client* troll = mockCommonClient();
	Client* channelOperator = mockOperatorClient();

	Server server;
	Channel *channel = new Channel("#world");

	// Add clients to the server clients list
	server._clients.push_back(*troll);
	server._clients.push_back(*channelOperator);

	// Add clients as channel member
	channel->invite(troll);
	channel->invite(channelOperator);

	// Set client channelOperator as channel operator	
	channel->set_channel_operator(channelOperator);
	
	// Add channel to the server channels list
	server._channels.push_back(channel);

	// Kick troll from the channel
	server._handler_client_kick("", 4);
	cr_assert(eq(int, server._reply_code, 461));
} */

Test(KickCommand, err_nosuchchannel)
{
	Client* troll = mockCommonClient();
	Client* channelOperator = mockOperatorClient();

	Server server;
	Channel *channel = new Channel("#world");

	// Add clients to the server clients list
	server._clients.push_back(*troll);
	server._clients.push_back(*channelOperator);

	// Add clients as channel member
	channel->invite(troll);
	channel->invite(channelOperator);

	// Set client channelOperator as channel operator	
	channel->set_channel_operator(channelOperator);
	
	// Add channel to the server channels list
	server._channels.push_back(channel);

	// Kick troll from the channel
	server._handler_client_kick("#deadworld trollUser", 4);
	cr_assert(eq(int, server._reply_code, 403));
}

Test(KickCommand, err_notonchannel)
{
	Client* troll = mockCommonClient();
	Client* channelOperator = mockOperatorClient();
	Client* outsideUser = mockOutsideClient();

	Server server;
	Channel *channel = new Channel("#world");

	// Add clients to the server clients list
	server._clients.push_back(*troll);
	server._clients.push_back(*channelOperator);
	server._clients.push_back(*outsideUser);

	// Add clients as channel member
	channel->invite(troll);
	channel->invite(channelOperator);

	// Set client channelOperator as channel operator	
	channel->set_channel_operator(channelOperator);
	
	// Add channel to the server channels list
	server._channels.push_back(channel);

	// Kick troll from the channel
	server._handler_client_kick("#world trollUser", 6);
	cr_assert(eq(int, server._reply_code, 442));
}

Test(KickCommand, err_chanoprivsneeded)
{
	Client* troll = mockCommonClient();
	Client* channelOperator = mockOperatorClient();

	Server server;
	Channel *channel = new Channel("#world");

	channelOperator->set_is_operator(false);

	// Add clients to the server clients list
	server._clients.push_back(*troll);
	server._clients.push_back(*channelOperator);

	// Add clients as channel member
	channel->invite(troll);
	channel->invite(channelOperator);
	
	// Add channel to the server channels list
	server._channels.push_back(channel);

	// Kick troll from the channel
	server._handler_client_kick("#world trollUser", 4);
	cr_assert(eq(int, server._reply_code, 482));
}

Test(KickCommand, err_nosuchnick)
{
	Client* troll = mockCommonClient();
	Client* channelOperator = mockOperatorClient();

	Server server;
	Channel *channel = new Channel("#world");

	channelOperator->set_is_operator(false);

	// Add clients to the server clients list
	server._clients.push_back(*troll);
	server._clients.push_back(*channelOperator);

	// Add clients as channel member
	channel->invite(troll);
	channel->invite(channelOperator);

	// Set client channelOperator as channel operator
	// WARNING: for some reason, the channel must be added
	// to the server channels list AFTER setting the operator
	channelOperator->set_is_operator(true);
	channel->set_channel_operator(channelOperator);
	
	// Add channel to the server channels list
	server._channels.push_back(channel);

	// Kick troll from the channel
	server._handler_client_kick("#world noUserInServer", 4);
	cr_assert(eq(int, server._reply_code, 401));
}

Test(KickCommand, err_usernotinchannel)
{
	Client* troll = mockCommonClient();
	Client* channelOperator = mockOperatorClient();
	Client* outsideUser = mockOutsideClient();

	Server server;
	Channel *channel = new Channel("#world");

	channelOperator->set_is_operator(false);

	// Add clients to the server clients list
	server._clients.push_back(*troll);
	server._clients.push_back(*channelOperator);
	server._clients.push_back(*outsideUser);

	// Add clients as channel member
	channel->invite(troll);
	channel->invite(channelOperator);

	// Set client channelOperator as channel operator
	// WARNING: for some reason, the channel must be added
	// to the server channels list AFTER setting the operator
	channelOperator->set_is_operator(true);
	channel->set_channel_operator(channelOperator);
	
	// Add channel to the server channels list
	server._channels.push_back(channel);

	// Kick troll from the channel
	server._handler_client_kick("#world outsideUser", 4);
	cr_assert(eq(int, server._reply_code, 441));
}
