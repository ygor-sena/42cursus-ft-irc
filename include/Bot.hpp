/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:31:32 by caalbert          #+#    #+#             */
/*   Updated: 2024/05/27 23:05:50 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include "Server.hpp"

class Bot {
public:
	static void _handle_marvin(Server &server, const std::string &buffer, int fd);
	static void _handle_time(Server &server, const std::string &buffer, int fd);
	static void _handle_whois(Server &server, const std::string &buffer, int fd);
};

#endif
