/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestPart.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 08:56:56 by yde-goes          #+#    #+#             */
/*   Updated: 2024/05/25 21:14:47 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "Channel.hpp"
#include "Client.hpp"
#define private public
#include "Server.hpp"

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

Test(PartCommand, part_successfully)
{
	Client* toPart = toPartClient();
	Client* channelMember = genericClient();

	Server server;

	Channel* channel = new Channel("#channel");

	server._clients.push_back(*toPart);
	server._clients.push_back(*channelMember);

	std::cout << channel->get_clients_size() << std::endl;

	channel->invite(toPart);
	channel->invite(channelMember);

	server._channels.push_back(channel);

	server._handler_client_part("#channel", toPart->get_fd());

	cr_assert(eq(int, server._reply_code, 200));
	cr_assert(eq(
		int, server._get_channel(channel->get_name())->get_clients_size(), 1));
}

Test(PartCommand, err_nosuchchannel)
{
	Client* toPart = toPartClient();
	Client* channelMember = genericClient();

	Server server;

	Channel* channel = new Channel("#channel");

	server._clients.push_back(*toPart);
	server._clients.push_back(*channelMember);

	std::cout << channel->get_clients_size() << std::endl;

	channel->invite(toPart);
	channel->invite(channelMember);

	server._channels.push_back(channel);

	server._handler_client_part("#nonExistent", toPart->get_fd());

	cr_assert(eq(int, server._reply_code, 403));
}

Test(PartCommand, err_notonchannel)
{
	Client* toPart = toPartClient();
	Client* channelMember = genericClient();

	Server server;

	Channel* channel = new Channel("#channel");

	server._clients.push_back(*toPart);
	server._clients.push_back(*channelMember);

	std::cout << channel->get_clients_size() << std::endl;

	channel->invite(channelMember);

	server._channels.push_back(channel);

	server._handler_client_part("#channel", toPart->get_fd());

	cr_assert(eq(int, server._reply_code, 442));
}

Test(PartCommand, err_notregistered)
{
	Client* toPart = toPartClient();
	Client* channelMember = genericClient();

	Server server;

	Channel* channel = new Channel("#channel");

	toPart->set_is_logged(false);

	server._clients.push_back(*toPart);
	server._clients.push_back(*channelMember);

	std::cout << channel->get_clients_size() << std::endl;

	channel->invite(channelMember);
	channel->invite(toPart);

	server._channels.push_back(channel);

	server._handler_client_part("#channel", toPart->get_fd());

	cr_assert(eq(int, server._reply_code, 451));
}
