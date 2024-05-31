/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestMode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 16:51:31 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/31 17:47:56 by gilmar           ###   ########.fr       */
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

/*
 * 1. O comando MODE é recebido sem parâmetros suficientes.
 */
Test(ModeCommand, err_needmoreparams)
{
	Client* client = mockClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_mode("#channel", client->get_fd());

	cr_assert(eq(int, server._reply_code, 461));
}

/*
 * 2. O comando MODE é recebido e o canal não existe.
 */
Test(ModeCommand, err_nosuchchannel)
{
	Client* client = mockClient();

	Server server;
	server._clients.push_back(client);
	server._handler_client_mode("#channel +i", client->get_fd());

	cr_assert(eq(int, server._reply_code, 403));
}

/*
 * 3. O comando MODE é recebido e o cliente não tem privilégios de operador no
 * canal.
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

/*
 * 4. O comando MODE é recebido e o modo do canal é inválido.
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

/*
 * 5. O comando MODE é recebido e o modo do canal é válido.
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

/*
 * 6. O comando MODE é recebido e o modo do canal é alterado com sucesso.
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

/*
 * 7. O comando MODE é recebido e o modo +i do canal é alterado com sucesso.
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

/*
 * 8. O comando MODE é recebido e o modo -i do canal é alterado com sucesso.
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

/*
 * 9. O comando MODE é recebido e o modo +t do canal é alterado com sucesso.
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

/*
 * 10. O comando MODE é recebido e o modo -t do canal é alterado com sucesso.
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

/*
 * 11. O comando MODE é recebido e o modo +k do canal é alterado com sucesso.
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

/*
 * 12. O comando MODE é recebido e o modo -k do canal é alterado com sucesso.
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

/*
 * 13. O comando MODE é recebido e o modo +l do canal é alterado com sucesso.
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

/*
 * 14. O comando MODE é recebido e o modo -l do canal é alterado com sucesso.
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

/*
 * 15. O comando MODE é recebido e o modo +o do canal é alterado com sucesso.
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

/*
 * 16. O comando MODE é recebido e o modo -o do canal é alterado com sucesso.
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
