/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MarvinBot.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:32:28 by caalbert          #+#    #+#             */
/*   Updated: 2024/05/29 19:19:32 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MarvinBot.hpp"
/*
** ------------------------------- COMMAND HANDLERS --------------------------------
*/

// Array of quote strings
const char* marvin_bot_quotes[QUOTES_COUNT] = {
    SOCRATES_KNOWLEDGE,
    DESCARTES_EXISTENCE,
    KANT_CATEGORICAL_IMPERATIVE,
    NIETZSCHE_GOD,
    PLATO_FORMS,
    MARX_RELIGION,
    CONFUCIUS_WISDOM,
    HUME_SCIENCE,
    HEIDEGGER_BEING,
    ROUSSEAU_FREEDOM
};

std::string _return_quote()
{
		// Seed the random number generator with the current time
		srand(time(0));

		// Generate a random index
		int index = rand() % QUOTES_COUNT;

		// Return the quote at the random index
		return std::string(marvin_bot_quotes[index]);
}

void Server::_handler_bot_marvin(const std::string &/* buffer */, int fd)
{
	Client* client = _get_client(fd);

	if (!client->get_is_logged())
	{
		_send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
		_reply_code = 451;
		return;
	} 
	else if (this->_is_client_in_any_channel(fd))
	{
		this->_send_response(fd, BOT_CMDMARVIN(client->get_nickname()));
		_reply_code = 4242;
	}
	else
	{
		this->_send_response(fd, BOT_CLIENTNOTINCHANNEL(client->get_nickname()));
        _reply_code = 2424;
    }
}

void Server::_handler_bot_time(const std::string &/* buffer */, int fd)
{
	Client* client = _get_client(fd);

	if (!client->get_is_logged())
	{
		_send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
		_reply_code = 451;
		return;
	} 
	else if (this->_is_client_in_any_channel(fd))
	{
		time_t now = time(NULL);
		std::string time_str = ctime(&now);
		this->_send_response(fd, BOT_CMDTIME(client->get_nickname(), time_str));
	}
	else
		this->_send_response(fd, BOT_CLIENTNOTINCHANNEL(client->get_nickname()));
}

void Server::_handler_bot_whoami(const std::string &/* buffer */, int fd)
{
	Client* client = _get_client(fd);

	if (!client->get_is_logged())
	{
		_send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
		_reply_code = 451;
		return;
	}
	else if (this->_is_client_in_any_channel(fd))
		this->_send_response(fd, BOT_CMDWHOIS(client->get_nickname(), client->get_username(), client->get_ip_address()));
	else
		this->_send_response(fd, BOT_CLIENTNOTINCHANNEL(client->get_nickname()));
}

void Server::_handler_bot_whois(const std::string &buffer, int fd)
{
	std::istringstream iss(buffer);
	std::string nickname;
	iss >> nickname;

	Client *client = _get_client(fd);
	Client *whois = _get_client(nickname);

	if (!client->get_is_logged())
	{
		_send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
		_reply_code = 451;
		return;
	}
	else if (this->_is_client_in_any_channel(fd))
		if (whois)
			this->_send_response(fd, BOT_CMDWHOIS(whois->get_nickname(), whois->get_username(), whois->get_ip_address()));
		else
			this->_send_response(fd, BOT_WHOISDOESNTEXIST(client->get_nickname()));
	else
		this->_send_response(fd, BOT_CLIENTNOTINCHANNEL(client->get_nickname()));
}

void Server::_handler_bot_quote(const std::string &/* buffer */, int fd)
{
	Client* client = _get_client(fd);

	if (!client->get_is_logged())
	{
		_send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
		_reply_code = 451;
		return;
	}
	else if (this->_is_client_in_any_channel(fd))
		this->_send_response(fd, BOT_CMDQUOTE(client->get_nickname(), _return_quote()));
	else
		this->_send_response(fd, BOT_CLIENTNOTINCHANNEL(client->get_nickname()));
}