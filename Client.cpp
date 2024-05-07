/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:20:02 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/01 16:35:53 by gilmar           ###   ########.fr       */
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

int Client::get_fd() const
{
    return _fd;
}

std::string Client::get_buffer() const
{
    return _buffer;
}