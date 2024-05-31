/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MarvinBot.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:32:28 by caalbert          #+#    #+#             */
/*   Updated: 2024/05/30 22:37:35 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MarvinBot.hpp"

// Constructor
MarvinBot::MarvinBot()
{
}

// Destructor
MarvinBot::~MarvinBot()
{
}

/**
 * @brief Array of quotes used by the MarvinBot class.
 *
 * This array stores the quotes used by the MarvinBot class. Each quote is represented as a C-style string.
 * The quotes are accessed using the predefined constants in the MarvinBot class.
 */
const char* marvin_bot_quotes[MarvinBot::QUOTES_COUNT] = {
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

/**
 * @brief Generates and returns a random quote from the list of quotes.
 *
 * @return A randomly selected quote.
 */
std::string _return_quote()
{
		srand(time(0));
		int index = rand() % MarvinBot::QUOTES_COUNT;

		return std::string(marvin_bot_quotes[index]);
}

/*
** ------------------------------- COMMAND HANDLERS --------------------------------
*/

/**
 * Handles the bot command "marvin" for a specific client.
 *
 * This function is responsible for processing the "marvin" command received from a client.
 * It checks if the client is logged in and if they are already in any channel. Depending on the
 * client's state, it sends the appropriate response back to the client.
 *
 * @param buffer The command buffer received from the client (not used in this function).
 * @param fd The file descriptor of the client's socket connection.
 */
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

/**
 * Handles the bot time command.
 *
 * This function is responsible for handling the bot time command received from a client.
 * It checks if the client is logged in and if they are in any channel. If the client is
 * not registered, it sends an error response. If the client is registered and in a channel,
 * it sends the current time to the client.
 *
 * @param buffer The command buffer received from the client.
 * @param fd The file descriptor of the client's socket.
 */
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

/**
 * Handles the "bot_whoami" command.
 * This function is responsible for processing the "bot_whoami" command received from a client.
 * It checks if the client is logged in and sends the appropriate response based on the client's status.
 *
 * @param buffer The command buffer (not used in this function).
 * @param fd The file descriptor of the client.
 */
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

/**
 * @brief Handles the BOT_WHOIS command from the server.
 *
 * This function is responsible for processing the BOT_WHOIS command received from the server.
 * It extracts the nickname from the buffer and retrieves the corresponding client object.
 * If the client is not logged in, it sends an error response.
 * If the client is logged in and is in a channel, it sends a response with the client's information.
 * If the client is logged in but not in a channel, it sends a response indicating that the client is not in a channel.
 * If the client does not exist, it sends a response indicating that the client does not exist.
 *
 * @param buffer The buffer containing the command and its arguments.
 * @param fd The file descriptor of the client connection.
 */
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

/**
 * Handles the bot quote command.
 *
 * This function is responsible for handling the bot quote command received from a client.
 * It checks if the client is logged in and sends an appropriate response based on the client's status.
 *
 * @param buffer The command buffer received from the client.
 * @param fd The file descriptor of the client's connection.
 */
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
