/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:23:47 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/31 23:12:41 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

//-------------------------------------------------------//

#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"

//-------------------------------------------------------//

#include "Channel.hpp"
#include "Client.hpp"
#include "Replies.hpp"

#define SPACE " "
#define CRLF "\r\n"
#define LINE_FEED "\n"
#define DELIMITER " \t"

class Server
{
  public:
	Server();
	~Server();
	
	// This contructor is to be used as a workaround for mockup tests
	Server(std::string password, std::vector<Client*> _clients,
		   std::vector<Channel*> _channels);

	void init(const std::string& port, const std::string& password);

  private:
	int _port;
	int _server_fdsocket;
	std::string _hostname;
	std::string _password;
	std::vector<Client*> _clients;
	std::vector<struct pollfd> _fds;
	struct sockaddr_in _server_addr;
	std::vector<Channel*> _channels;

	int _reply_code;  // -> This a workaround to test the IRC commands without
					  // mocking up a client-server connection

	std::string _get_hostname();
	void _is_valid_port(const std::string& port);
	bool _is_valid_nickname(const std::string& nickname);
	bool _is_nickname_in_use(const int fd, const std::string& nickname);
	bool _is_client_in_any_channel(const int fd);

	void _handler_client_join(const std::string& buffer, const int fd);
	void _handler_client_quit(const std::string& buffer, const int fd);
	void _handler_client_part(const std::string& buffer, const int fd);
	void _handler_client_mode(const std::string& buffer, const int fd);
	void _handler_client_kick(const std::string& buffer, const int fd);
	void _handler_client_topic(const std::string& buffer, const int fd);
	void _handler_client_invite(const std::string& buffer, const int fd);
	void _handler_client_privmsg(const std::string& buffer, const int fd);
	void _handler_client_nickname(const std::string& nickname, const int fd);
	void _handler_client_username(const std::string& username, const int fd);
	void _handler_client_password(const std::string& password, const int fd);

	void _handler_bot_marvin(const std::string& buffer, int fd);
	void _handler_bot_time(const std::string& buffer, int fd);
	void _handler_bot_whois(const std::string& buffer, int fd);
	void _handler_bot_whoami(const std::string& buffer, int fd);
	void _handler_bot_quote(const std::string& buffer, int fd);

	void _set_channel_limit_mode(Channel* channel, const std::string& limitStr,
								 bool addMode);
	void _set_channel_operator_mode(Channel* channel, Client* client,
									bool addMode);
	void _set_channel_key_mode(Channel* channel, const std::string& key,
							   bool addMode);
	void _set_topic_restriction_mode(Channel* channel, bool addMode);
	void _set_invite_only_mode(Channel* channel, bool addMode);

	bool _process_mode_flags(const std::string& modeFlags, Channel* channel,
							 Client* targetClient, const std::string& argument,
							 const int fd);
	bool _apply_mode_flag(Channel* channel, Client* targetClient, char mode,
						  bool addMode, const std::string& argument);

	static bool _signal;
	static void _signal_handler(const int signum);

	void _server_loop();
	void _set_server_socket();
	void _add_server_signal();
	void _accept_new_client();
	void _clear_client(const int fd);
	void _receive_new_data(const int fd);
	void _send_response(
		const int fd,
		const std::string& response);

	struct command_handler
	{
		std::string command;
		void (Server::*handler)(const std::string&, const int);
	};

	static const int _command_list_size = 16;
	static const command_handler
		_command_list[_command_list_size];

	void _execute_command(const std::string buffer,
						  const int fd);
	void _close_fds();

	std::string _cleanse_buffer(
		const std::string& buffer,
		const std::string& chars_to_remove);
	std::vector<std::string> _split_buffer(
		const std::string& buffer,
		const std::string& delimiter);

	Client* _get_client(const int fd);
	Client* _get_client(const std::string nickname);
	Channel* _get_channel(const std::string& channel_name);
	void _add_channel(
		Channel* channel);
	bool _client_is_ready_to_login(const int fd);

	std::string toupper(const std::string& str);

	void _remove_client_from_channels(const int fd);
	void _remove_client_from_server(const int fd);
	void _remove_client_fd(const int fd);
};

#endif	// SERVER_HPP