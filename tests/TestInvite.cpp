/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestInvite.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:15:32 by yde-goes          #+#    #+#             */
/*   Updated: 2024/05/25 10:23:06 by yde-goes         ###   ########.fr       */
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
	client->set_fd(5);
	client->set_username("outsideUser");
	client->set_nickname("outsideUser");
	client->set_password("password");
	client->set_buffer("");
	client->set_is_registered(true);
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	return client;
}

Client *mockInsideClient()
{
	Client *client = new Client();
	client->set_fd(4);
	client->set_username("insideUser");
	client->set_nickname("insideUser");
	client->set_password("password");
	client->set_buffer("");
	client->set_is_registered(true);
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	client->set_is_operator(false);
	return client;
}

Test(InviteCommand, succesfully_invite_client_to_channel)
{
	Client* outsideClient = mockOutsideClient();
	Client* insideClient = mockInsideClient();

	Server server;
	Channel *channel = new Channel("#world");

	channel->set_channel_operator(insideClient);

	// Add clients to the server clients list
    server._clients.push_back(*outsideClient);
	server._clients.push_back(*insideClient);

	// Add clients as channel member
	channel->invite(insideClient);

	// Add channel to the server channels list
    server._channels.push_back(channel);

	// Invite client to channel
	server._handler_client_invite("outsideUser #world", 4);
	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int, server._get_channel(channel->get_name())->get_clients_size(), 2));
}

Test(InviteCommand, err_nosuchchannel)
{
	Client* outsideClient = mockOutsideClient();
	Client* insideClient = mockInsideClient();

	Server server;
	Channel *channel = new Channel("#world");

	// Add clients to the server clients list
    server._clients.push_back(*outsideClient);
	server._clients.push_back(*insideClient);
	
	// Add channel to the server channels list
    server._channels.push_back(channel);

	// Invite client to channel
	server._handler_client_invite("outsideClient #deadworld", 4);
	cr_assert(eq(int, server._reply_code, 403));
}

Test(InviteCommand, err_notonchannel)
{
	Client* outsideClient = mockOutsideClient();
	Client* insideClient = mockInsideClient();

	Server server;
	Channel *channel = new Channel("#world");

	// Add clients to the server clients list
    server._clients.push_back(*outsideClient);
	server._clients.push_back(*insideClient);

	channel->invite(outsideClient);
	
	// Add channel to the server channels list
    server._channels.push_back(channel);

	// Invite client to channel
	server._handler_client_invite("outsideClient #world", 4);
	cr_assert(eq(int, server._reply_code, 442));
}

Test(InviteCommand, err_nosuchnick)
{
	Client* insideClient = mockInsideClient();

	Server server;
	Channel *channel = new Channel("#world");

	channel->set_channel_operator(insideClient);

	// Add clients to the server clients list
	server._clients.push_back(*insideClient);

	channel->invite(insideClient);
	
	// Add channel to the server channels list
    server._channels.push_back(channel);

	// Invite client to channel
	server._handler_client_invite("outsideClient #world", 4);
	cr_assert(eq(int, server._reply_code, 401));
}

Test(InviteCommand, err_noprivileges)
{
	Client* insideClient = mockInsideClient();

	Server server;
	Channel *channel = new Channel("#world");

	// Add clients to the server clients list
	server._clients.push_back(*insideClient);

	// Add clients as channel member
	channel->invite(insideClient);

	// Add channel to the server channels list
    server._channels.push_back(channel);

	// Invite client to channel
	server._handler_client_invite("outsideUser #world", 4);
	cr_assert(eq(int, server._reply_code, 481));
}
	
Test(InviteCommand, err_useronchannel)
{
	Client* insideClient = mockInsideClient();

	Server server;
	Channel *channel = new Channel("#world");

	channel->set_channel_operator(insideClient);

	// Add clients to the server clients list
	server._clients.push_back(*insideClient);

	channel->invite(insideClient);
	
	// Add channel to the server channels list
    server._channels.push_back(channel);

	// Invite client to channel
	server._handler_client_invite("insideUser #world", 4);
	cr_assert(eq(int, server._reply_code, 443));
}

Test(InviteCommand, err_notregistered)
{
	Client* insideClient = mockInsideClient();

	Server server;
	Channel *channel = new Channel("#world");

	insideClient->set_is_registered(false);

	// Add clients to the server clients list
	server._clients.push_back(*insideClient);

	channel->invite(insideClient);
	
	// Add channel to the server channels list
    server._channels.push_back(channel);

	// Invite client to channel
	server._handler_client_invite("insideUser #world", 4);
	cr_assert(eq(int, server._reply_code, 451));
}
