/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:20:02 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/11 18:11:04 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client()
{
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

void Client::set_is_registered(const bool is_registered)
{
    _is_registered = is_registered;
}



bool Client::get_is_registered() const
{
    return _is_registered;
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