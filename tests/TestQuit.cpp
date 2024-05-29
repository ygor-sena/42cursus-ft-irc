/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestQuit.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 20:46:13 by yde-goes          #+#    #+#             */
/*   Updated: 2024/05/28 21:32:43 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "Client.hpp"
#include "Channel.hpp"
#define private public
#include "Server.hpp"

Client *genericClient()
{
	Client *client = new Client();
	client->set_fd(6);
	client->set_username("soAndSoUser");
	client->set_nickname("soAndSoUser");
	client->set_password("password");
	client->set_buffer("");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	return client;
}

Client *toPartClient()
{
	Client *client = new Client();
	client->set_fd(4);
	client->set_username("toPartUser");
	client->set_nickname("toPartUser");
	client->set_password("password");
	client->set_buffer("");
	client->set_is_logged(true);
	client->set_is_authenticated(true);
	return client;
}

/* Test(QuitCommand, non_client_operator_quits_successfully)
{
	Client *toPart = toPartClient();
	Client *channelMember = genericClient();

	Server server;

	std::vector<Channel*> channels = {
		new Channel("#one"), new Channel("#two"), new Channel("#three")
	};

	toPart->set_is_operator(false);

	server._clients.push_back(*toPart);
	server._clients.push_back(*channelMember);

	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
		(*it)->join(toPart);
		(*it)->join(channelMember);
		server._channels.push_back(*it);
	}

	server._handler_client_quit("It's time to go", toPart->get_fd());

	cr_assert(eq(int, server._reply_code, 301));
	for (std::vector<Channel*>::iterator it = server._channels.begin(); it != server._channels.end(); ++it) {
		cr_assert(eq(int, (*it)->get_clients_size(), 1));
	}
}
 */
Test(QuitCommand, client_operator_quits_successfully)
{
	Client *toPart = toPartClient();
	Client *channelMember = genericClient();

	Server server;

	std::vector<Channel*> channels = {
		new Channel("#one"), new Channel("#two"), new Channel("#three")
	};

	toPart->set_is_operator(false);

	server._clients.push_back(*toPart);
	server._clients.push_back(*channelMember);

	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
		(*it)->join(toPart);
		(*it)->join(channelMember);
		//(*it)->set_channel_operator(toPart);
		server._channels.push_back(*it);
	}

	server._handler_client_quit("It's time to go", toPart->get_fd());

	std::cout << "clients after quit: " << server._get_channel("#one")->get_client_names() << std::endl;

	cr_assert(eq(int, server._reply_code, 301));
	for (std::vector<Channel*>::iterator it = server._channels.begin(); it != server._channels.end(); ++it) {
		cr_assert(eq(int, (*it)->get_clients_size(), 1));
	}
}
