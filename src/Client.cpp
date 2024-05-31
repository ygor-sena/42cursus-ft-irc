/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:20:02 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/30 23:17:50 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client()
{
	_fd = -1;
	_is_logged = false;
	_is_authenticated = false;
	_is_operator = false;
	_buffer = "";
	_ip_addr = "";
	_nickname = "";
	_username = "";
	_password = "";
	_channels_invited = std::vector<std::string>();
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/**
 * @brief Sets the file descriptor for the client.
 *
 * @param fd The file descriptor to set.
 */
void Client::set_fd(const int fd) { _fd = fd; }

/**
 * @brief Sets the IP address for the client.
 *
 * This function sets the IP address for the client to the specified value.
 *
 * @param ipadd The IP address to set.
 */
void Client::set_ip_add(const std::string& ipadd) { _ip_addr = ipadd; }

/**
 * @brief Get the IP address of the client.
 *
 * @return std::string The IP address of the client.
 */
std::string Client::get_ip_address() const { return _ip_addr; }

/**
 * @brief Sets the buffer of the client.
 *
 * This function sets the buffer of the client to the specified value.
 *
 * @param buffer The new buffer value.
 */
void Client::set_buffer(const std::string& buffer) { _buffer = buffer; }

/**
 * @brief Sets the nickname of the client.
 *
 * This function sets the nickname of the client to the specified value.
 *
 * @param nickname The nickname to set.
 */
void Client::set_nickname(const std::string& nickname) { _nickname = nickname; }

/**
 * @brief Sets the username of the client.
 *
 * This function sets the username of the client to the specified value.
 *
 * @param username The username to set.
 */
void Client::set_username(const std::string& username) { _username = username; }

/**
 * @brief Sets the password for the client.
 *
 * This function sets the password for the client. The password is used for
 * authentication purposes when connecting to a server.
 *
 * @param password The password to set.
 */
void Client::set_password(const std::string& password) { _password = password; }

/**
 * @brief Sets the authentication status of the client.
 *
 * This function sets the authentication status of the client to the specified
 * value.
 *
 * @param is_authenticated The authentication status to set.
 */
void Client::set_is_authenticated(bool is_authenticated)
{
	_is_authenticated = is_authenticated;
}

/**
 * @brief Sets the operator status of the client.
 *
 * This function sets the operator status of the client. An operator is a user
 * with special privileges and permissions in the IRC server.
 *
 * @param is_operator A boolean value indicating whether the client is an
 * operator or not.
 */
void Client::set_is_operator(bool is_operator) { _is_operator = is_operator; }

/**
 * @brief Sets the logged status of the client.
 *
 * This function sets the logged status of the client to the specified value.
 *
 * @param is_logged The new logged status of the client.
 */
void Client::set_is_logged(bool is_logged) { _is_logged = is_logged; }

/**
 * @brief Get the authentication status of the client.
 *
 * @return true if the client is authenticated, false otherwise.
 */
bool Client::get_is_authenticated() const { return _is_authenticated; }

/**
 * @brief Get the operator status of the client.
 *
 * @return true if the client is an operator, false otherwise.
 */
bool Client::get_is_operator() const { return _is_operator; }

/**
 * @brief Get the logged status of the client.
 *
 * @return true if the client is logged in, false otherwise.
 */
bool Client::get_is_logged() const { return _is_logged; }

/**
 * @brief Get the file descriptor of the client.
 *
 * @return The file descriptor of the client.
 */
int Client::get_fd() const { return _fd; }

/**
 * @brief Get the buffer of the client.
 *
 * This function returns the buffer of the client, which contains the received
 * data.
 *
 * @return std::string The buffer of the client.
 */
std::string Client::get_buffer() const { return _buffer; }

/**
 * @brief Get the nickname of the client.
 *
 * @return std::string The nickname of the client.
 */
std::string Client::get_nickname() const { return _nickname; }

/**
 * @brief Get the username of the client.
 *
 * @return std::string The username of the client.
 */
std::string Client::get_username() const { return _username; }

/**
 * @brief Get the password of the client.
 *
 * @return std::string The password of the client.
 */
std::string Client::get_password() const { return _password; }

/**
 * @brief Get the hostname of the client.
 *
 * This function returns the hostname of the client by concatenating the
 * nickname and IP address.
 *
 * @return std::string The hostname of the client.
 */
std::string Client::get_hostname() const { return _nickname + "@" + _ip_addr; }

/**
 * @brief Get the list of channels the client has been invited to.
 *
 * @return std::vector<std::string> The list of channels.
 */
std::vector<std::string> Client::get_channels_invited() const
{
	return _channels_invited;
}

/**
 * @brief Removes a channel from the list of invited channels.
 *
 * This function removes the specified channel from the list of channels
 * that the client has been invited to join.
 *
 * @param channel The name of the channel to remove.
 */
/**
 * @brief Removes a channel from the list of invited channels.
 *
 * This function removes the specified channel from the list of channels
 * that the client has been invited to join.
 *
 * @param channel The name of the channel to be removed.
 */
void Client::remove_channel_invited(const std::string& channel)
{
	std::vector<std::string>::iterator it;
	for (it = _channels_invited.begin(); it != _channels_invited.end(); ++it)
	{
		if (*it == channel)
		{
			_channels_invited.erase(it);
			break;
		}
	}
}

/**
 * @brief Adds a channel to the list of invited channels.
 *
 * This function adds the specified channel to the list of channels that the
 * client has been invited to.
 *
 * @param channel The name of the channel to be added.
 */
void Client::add_channel_invited(const std::string& channel)
{
	_channels_invited.push_back(channel);
}

/**
 * @brief Checks if the client is invited to a specific channel.
 *
 * This function checks if the client is invited to the specified channel.
 *
 * @param channel The name of the channel to check.
 * @return True if the client is invited to the channel, false otherwise.
 */
bool Client::is_channel_invited(const std::string& channel)
{
	return std::find(_channels_invited.begin(),
					 _channels_invited.end(),
					 channel) != _channels_invited.end();
}

/**
 * @brief Sends a broadcast message from the client to a target.
 *
 * This function constructs a response message using the sender's hostname,
 * target, and message. It then sends the response message to the client's file
 * descriptor using the send() function. If the send() function fails, an error
 * message is printed to the standard error stream.
 *
 * @param sender A pointer to the client sending the broadcast message.
 * @param target The target of the broadcast message.
 * @param message The content of the broadcast message.
 */
void Client::broadcast(Client* sender, std::string target, std::string message)
{
	std::string response = RPL_PRIVMSG(sender->get_hostname(), target, message);

	if (send(this->get_fd(), response.c_str(), response.size(), 0) == -1)
		std::cerr << "Response send() failed" << std::endl;
}
