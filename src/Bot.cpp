/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:32:28 by caalbert          #+#    #+#             */
/*   Updated: 2024/05/27 23:10:02 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <ctime>

void Bot::_handle_marvin(Server &server, int fd) {
	std::string response = ":Server 001 " + server.get_client(fd)->get_nickname() + " :Olá, sou Marvin, o robô paranóide.";
	server.send_response(fd, response + "\r\n");
}

void Bot::_handle_time(Server &server, int fd) {
	time_t now = time(NULL);
	std::string time_str = ctime(&now);
	std::string response = ":Server 001 " + server.get_client(fd)->get_nickname() + " :Server time: " + time_str;
	server.send_response(fd, response + "\r\n");
}

void Bot::_handle_whois(Server &server, int fd) {
	std::istringstream iss(buffer);
	std::string nickname;
	iss >> nickname;
	// Client* client = server.get_client(nickname);
	Client* client = server.get_client(fd);
	if (client) {
		std::string response = ":Server 311 " + client->get_nickname() + " " + client->get_username() + " " + client->get_ip_address() + " * :Real Name";
		server.send_response(fd, response + "\r\n");
		response = ":Server 318 " + client->get_nickname() + " :End of WHOIS list";
		server.send_response(fd, response + "\r\n");
	} else {
		// server.send_response(fd, server.ERR_NOSUCHNICK(nickname) + "\r\n");

	}
}

