/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:23:47 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/30 23:45:41 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>	 //-> for inet_ntoa()
#include <fcntl.h>		 //-> for fcntl()
#include <netinet/in.h>	 //-> for sockaddr_in
#include <poll.h>		 //-> for poll()
#include <sys/socket.h>	 //-> for socket()
#include <sys/types.h>	 //-> for socket()
#include <unistd.h>		 //-> for close()
#include <csignal>		 //-> for signal()
#include <cstdlib>		 // para a função std::atoi
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>		//-> for map
#include <sstream>	//-> for std::stringstream
#include <string>	// Para std::to_string
#include <vector>	//-> for vector

//-------------------------------------------------------//
#define RED "\e[1;31m"	//-> for red color
#define WHI "\e[0;37m"	//-> for white color
#define GRE "\e[1;32m"	//-> for green color
#define YEL "\e[1;33m"	//-> for yellow color
//-------------------------------------------------------//

#include "Channel.hpp"	//-> for channel class
#include "Client.hpp"	//-> for client class
#include "Replies.hpp"	//-> for replies class

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
	Server(std::string password, std::vector<Client> _clients,
		   std::vector<Channel> _channels);

	void init(const std::string& port, const std::string& password);

  private:
	int _port;						  //-> server port
	int _server_fdsocket;			  //-> server socket file descriptor
	std::string _password;			  //-> server password
	std::vector<Client> _clients;	  //-> vector of clients
	std::vector<struct pollfd> _fds;  //-> vector of pollfd
	struct sockaddr_in _server_addr;  //-> server address
	std::vector<Channel*> _channels;  //-> vector of channels

	int _reply_code;  // -> This a workaround to test the IRC commands without
					  // mocking up a client-server connection

	int get_reply_code(void);  // -> Get the reply code to be checked against
							   // criterion test results
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

	void _handler_invite_only_mode(Channel* channel, bool set);
	void _handler_topic_restriction_mode(Channel* channel, bool set);
	void _handle_operator_privileges_mode(Channel* channel, Client* client,
										  bool set);
	void _handle_password_mode(Channel* channel, const std::string& argument,
							   bool set);
	void _handle_limit_mode(Channel* channel, Client* client,
							const std::string& argument, bool set,
							const int fd);
	bool _handler_mode_flags(Channel* channel, Client* client, char mode,
							 bool set, const std::string& arg, const int fd);
	bool _parse_mode_command(const std::string& modes, Channel* channel,
							 Client* client, const std::string& arg,
							 const int fd);

	static bool _signal;  //-> static boolean for signal
	static void _signal_handler(const int signum);

	void _server_loop();				   //-> server loop
	void _set_server_socket();			   //-> server socket creation
	void _add_server_signal();			   //-> server signal creation
	void _accept_new_client();			   //-> accept new client
	void _clear_client(const int fd);	   //-> clear clients
	void _receive_new_data(const int fd);  //-> receive data from a client
	void _send_response(
		const int fd,
		const std::string& response);  //-> send response to client

	struct command_handler
	{
		std::string command;
		void (Server::*handler)(const std::string&, const int);
	};

	static const int _command_list_size = 16;  //-> command list size
	static const command_handler
		_command_list[_command_list_size];	//-> command list
	void _execute_command(const std::string buffer,
						  const int fd);  //-> execute command

	void _close_fds();	//-> close file descriptors
	std::string _cleanse_buffer(
		const std::string& buffer,
		const std::string& chars_to_remove);  //-> parse received buffer

	std::vector<std::string> _split_buffer(
		const std::string& buffer,
		const std::string& delimiter);	//-> split string

	Client* _get_client(const int fd);	//-> get client
	void _add_channel(
		Channel* channel);	// -> add a new channel to server channels
	bool _client_is_ready_to_login(const int fd);
	Client* _get_client(const std::string nickname);		 // -> get nickname
	Channel* _get_channel(const std::string& channel_name);	 //-> get channel

	std::string toupper(const std::string& str);
};

#endif	// SERVER_HPP