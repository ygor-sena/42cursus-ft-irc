/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:20:02 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/26 19:10:24 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client()
{
	_fd = -1;
	_ip_addr = "";
	_nickname = "";
	_username = "";
	_password = "";
	_buffer = "";
	_is_logged = false;
	_is_authenticated = false;
	_is_operator = false;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void Client::set_fd(const int fd)
{
	_fd = fd;
}

void Client::set_ip_add(const std::string &ipadd)
{
	_ip_addr = ipadd;
}

void Client::set_buffer(const std::string &buffer)
{
	_buffer = buffer;
}

void Client::set_nickname(const std::string &nickname)
{
	_nickname = nickname;
}

void Client::set_username(const std::string &username)
{
	_username = username;
}

void Client::set_password(const std::string &password)
{
	_password = password;
}

void Client::set_is_authenticated(bool is_authenticated)
{
	_is_authenticated = is_authenticated;
}

void Client::set_is_operator(bool is_operator)
{
	_is_operator = is_operator;
}

void Client::set_is_logged(bool is_logged)
{
	_is_logged = is_logged;
}

bool Client::get_is_authenticated() const
{
	return _is_authenticated;
}

bool Client::get_is_operator() const
{
	return _is_operator;
}

bool Client::get_is_logged() const
{
	return _is_logged;
}

int Client::get_fd() const
{
	return _fd;
}

std::string Client::get_buffer() const
{
	return _buffer;
}

std::string Client::get_nickname() const
{
	return _nickname;
}

std::string Client::get_username() const
{
	return _username;
}

std::string Client::get_password() const
{
	return _password;
}

std::string Client::get_hostname() const
{
	return _nickname + "@" + _ip_addr;
}

void Client::add_channel_invited(const std::string &channel)
{
	_channels_invited.push_back(channel);
}

bool Client::is_channel_invited(const std::string &channel)
{
	return std::find(_channels_invited.begin(), _channels_invited.end(), channel) != _channels_invited.end();
}

void Client::broadcast(Client *sender, std::string command, std::string target, std::string message)
{
	(void)command;
	std::string response = RPL_PRIVMSG(sender->get_hostname(), target, message);

	if (send(sender->get_fd(), response.c_str(), response.size(), 0) == -1)
		std::cerr << "Response send() failed" << std::endl;

  //_send_response(fd, RPL_PRIVMSG(client->get_hostname(),
}
