/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:19:06 by gilmar            #+#    #+#             */
/*   Updated: 2024/06/01 17:01:24 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <csignal>
#include <iostream>
#include <string>
#include <vector>
#include "Replies.hpp"

//-------------------------------------------------------//

#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"

//-------------------------------------------------------//

/**
 * @brief The Client class represents a client connected to the IRC server.
 *
 * This class provides methods to get and set various properties of the client,
 * such as file descriptor, login status, operator status, authentication status,
 * buffer, nickname, username, password, hostname, IP address, registration status,
 * and channels invited.
 *
 * It also provides methods to check if the client is invited to a specific channel,
 * add a channel to the list of invited channels, and remove a channel from the list
 * of invited channels.
 *
 * Additionally, it provides a method to broadcast a message to all clients except
 * the sender.
 */
class Client
{
  public:
	Client();
	~Client();

	Client(const Client& other);

	int get_fd() const;

	bool get_is_logged() const;
	bool get_is_operator() const;
	bool get_is_authenticated() const;
	std::string get_buffer() const;
	std::string get_nickname() const;
	std::string get_username() const;
	std::string get_password() const;
	std::string get_hostname() const;
	std::string get_ip_address() const;
	bool get_already_registered() const;
	std::vector<std::string> get_channels_invited() const;

	void set_fd(const int fd);
	void set_is_logged(bool is_logged);
	void set_is_operator(bool is_operator);
	void set_is_authenticated(bool is_authenticated);
	void set_buffer(const std::string& buffer);
	void set_nickname(const std::string& nickname);
	void set_username(const std::string& username);
	void set_password(const std::string& password);
	void set_ip_add(const std::string& ipadd);

	bool is_channel_invited(const std::string& channel);
	void add_channel_invited(const std::string& channel);
	void remove_channel_invited(const std::string& channel);

	void broadcast(Client* sender, std::string target, std::string message);

	void append_to_buffer(const std::string& data);
	void clear_buffer();

  private:
	int _fd;
	bool _is_logged;
	bool _is_authenticated;
	bool _is_operator;
	std::string _buffer;
	std::string _ip_addr;
	std::string _nickname;
	std::string _username;
	std::string _password;
	std::vector<std::string> _channels_invited;
};

#endif	// CLIENT_HPP