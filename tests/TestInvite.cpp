/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestInvite.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:15:32 by yde-goes          #+#    #+#             */
/*   Updated: 2024/06/01 09:00:39 by gilmar           ###   ########.fr       */
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
	client->set_fd(5);
	client->set_username("outsideUser");
	client->set_nickname("outsideUser");
	client->set_password("password");
	client->set_buffer("");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	return client;
}

Client* mockInsideClient()
{
	Client* client = new Client();
	client->set_fd(4);
	client->set_username("insideUser");
	client->set_nickname("insideUser");
	client->set_password("password");
	client->set_buffer("");
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
	Channel* channel = new Channel("#world");

	channel->set_channel_operator(insideClient);

	server._clients.push_back(outsideClient);
	server._clients.push_back(insideClient);

	channel->join(insideClient);

	server._channels.push_back(channel);

	server._handler_client_invite("outsideUser #world", 4);

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(int,
				 server._get_client(outsideClient->get_fd())
					 ->is_channel_invited(channel->get_name()),
				 1));
}

Test(InviteCommand, err_nosuchchannel)
{
	Client* outsideClient = mockOutsideClient();
	Client* insideClient = mockInsideClient();

	Server server;
	Channel* channel = new Channel("#world");

	server._clients.push_back(outsideClient);
	server._clients.push_back(insideClient);

	server._channels.push_back(channel);

	server._handler_client_invite("outsideClient #deadworld", 4);
	cr_assert(eq(int, server._reply_code, 403));
}

Test(InviteCommand, err_notonchannel)
{
	Client* outsideClient = mockOutsideClient();
	Client* insideClient = mockInsideClient();

	Server server;
	Channel* channel = new Channel("#world");

	server._clients.push_back(outsideClient);
	server._clients.push_back(insideClient);

	channel->invite(outsideClient);

	server._channels.push_back(channel);

	server._handler_client_invite("outsideClient #world", 4);
	cr_assert(eq(int, server._reply_code, 442));
}

Test(InviteCommand, err_nosuchnick)
{
	Client* insideClient = mockInsideClient();

	Server server;
	Channel* channel = new Channel("#world");

	channel->set_channel_operator(insideClient);

	server._clients.push_back(insideClient);

	channel->invite(insideClient);

	server._channels.push_back(channel);

	server._handler_client_invite("outsideClient #world", 4);
	cr_assert(eq(int, server._reply_code, 401));
}

Test(InviteCommand, err_noprivileges)
{
	Client* insideClient = mockInsideClient();

	Server server;
	Channel* channel = new Channel("#world");

	server._clients.push_back(insideClient);

	channel->invite(insideClient);

	server._channels.push_back(channel);

	server._handler_client_invite("outsideUser #world", 4);
	cr_assert(eq(int, server._reply_code, 481));
}

Test(InviteCommand, err_useronchannel)
{
	Client* insideClient = mockInsideClient();

	Server server;
	Channel* channel = new Channel("#world");

	channel->set_channel_operator(insideClient);

	server._clients.push_back(insideClient);

	channel->invite(insideClient);

	server._channels.push_back(channel);

	server._handler_client_invite("insideUser #world", 4);
	cr_assert(eq(int, server._reply_code, 443));
}

Test(InviteCommand, err_notregistered)
{
	Client* insideClient = mockInsideClient();

	Server server;
	Channel* channel = new Channel("#world");

	insideClient->set_is_logged(false);

	server._clients.push_back(insideClient);

	channel->invite(insideClient);

	server._channels.push_back(channel);

	server._handler_client_invite("insideUser #world", 4);
	cr_assert(eq(int, server._reply_code, 451));
}
