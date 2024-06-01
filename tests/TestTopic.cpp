/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestTopic.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 08:57:00 by yde-goes          #+#    #+#             */
/*   Updated: 2024/05/31 21:17:06 by gilmar           ###   ########.fr       */
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

/*
 * 1. O comando TOPIC é recebido sem parâmetros suficientes.
*/
Test(TopicCommand, err_needmoreparams)
{
	Client* client = mockChannelMemberClient();
	
	Server server;
	server._clients.push_back(client);
	server._handler_client_topic("", client->get_fd());

	cr_assert(eq(int, server._reply_code, 461));
}

/*
 * 2. O comando TOPIC é recebido e o cliente não está logado.
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

/*
 * 3. O comando TOPIC é recebido e o canal não existe.
*/
Test(TopicCommand, err_nosuchchannel)
{
	Client* client = mockChannelMemberClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_topic("#channel", client->get_fd());
	
	cr_assert(eq(int, server._reply_code, 403));
}

/*
 * 4. O comando TOPIC é recebido e o cliente não está no canal.
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

/*
 * 5. O comando TOPIC é recebido e o canal não tem tópico.
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

/*
 * 6. O comando TOPIC é recebido e o canal tem tópico.
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

/*
 * 7. O comando TOPIC é recebido e o cliente não é operador do canal.
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

/*
 * 8. O comando TOPIC é recebido e o canal tem restrição de tópico.
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

/*
 * 9. O comando TOPIC é recebido e o tópico é alterado com sucesso.
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
