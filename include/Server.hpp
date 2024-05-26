/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:23:47 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/25 16:39:43 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string> // Para std::to_string
# include <cstring>
# include <cstdlib> // para a função std::atoi
# include <iostream>
# include <vector> //-> for vector
# include <sys/socket.h> //-> for socket()
# include <sys/types.h> //-> for socket()
# include <netinet/in.h> //-> for sockaddr_in
# include <fcntl.h> //-> for fcntl()
# include <unistd.h> //-> for close()
# include <arpa/inet.h> //-> for inet_ntoa()
# include <poll.h> //-> for poll()
# include <csignal> //-> for signal()
# include <sstream> //-> for std::stringstream
# include <iomanip>
# include <map> //-> for map
# include <functional> //-> for std::function

//-------------------------------------------------------//
# define RED "\e[1;31m" //-> for red color
# define WHI "\e[0;37m" //-> for white color
# define GRE "\e[1;32m" //-> for green color
# define YEL "\e[1;33m" //-> for yellow color
//-------------------------------------------------------//

# include "Client.hpp" //-> for client class
# include "Replies.hpp" //-> for replies class
# include "Channel.hpp" //-> for channel class

# define CRLF "\r\n"
# define LINE_FEED "\n"
# define DELIMITER " \t"

class Server
{
	public:
		Server();
		~Server();
		// This contructor is to be used as a workaround for mockup tests
		Server(std::string password, std::vector<Client> _clients, std::vector<Channel> _channels);

		void init(const std::string &port, const std::string &password);

	private:
		int _port; //-> server port
		int _server_fdsocket; //-> server socket file descriptor
		std::string _password; //-> server password
		std::vector<Client> _clients; //-> vector of clients
		std::vector<struct pollfd> _fds; //-> vector of pollfd
		struct sockaddr_in _server_addr; //-> server address
		std::vector<Channel *> _channels; //-> vector of channels

		int _reply_code; // -> This a workaround to test the IRC commands without mocking up a client-server connection

		int get_reply_code(void); // -> Get the reply code to be checked against criterion test results
		void _is_valid_port(const std::string &port);
		bool _is_valid_nickname(const std::string &nickname);
		bool _is_nickname_in_use(const int fd, const std::string &nickname);

		void _handler_client_join(const std::string &buffer, const int fd);
		void _handler_client_quit(const std::string &buffer, const int fd);
		void _handler_client_part(const std::string &buffer, const int fd);
		void _handler_client_mode(const std::string &buffer, const int fd);
		void _handler_client_kick(const std::string &buffer, const int fd);
		void _handler_client_topic(const std::string &buffer, const int fd);
		void _handler_client_invite(const std::string &buffer, const int fd);
		void _handler_client_privmsg(const std::string &buffer, const int fd);
		void _handler_client_nickname(const std::string &nickname, const int fd);
		void _handler_client_username(const std::string &username, const int fd);
		void _handler_client_password(const std::string &password, const int fd);

		static bool _signal; //-> static boolean for signal
		static void _signal_handler(const int signum);

		void _server_loop(); //-> server loop
		void _set_server_socket(); //-> server socket creation
		void _add_server_signal(); //-> server signal creation
		void _accept_new_client(); //-> accept new client
		void _clear_client(const int fd); //-> clear clients
		void _receive_new_data(const int fd); //-> receive data from a client
		void _send_response(const int fd, const std::string &response); //-> send response to client
		
		struct command_handler
		{
			std::string command;
			void (Server::*handler)(const std::string &, const int);
		};
		
		static const int _command_list_size = 12; //-> command list size
		static const command_handler _command_list[_command_list_size]; //-> command list
		void _execute_command(const std::string buffer, const int fd); //-> execute command
		
		std::string _cleanse_buffer(const std::string &buffer, const std::string &chars_to_remove); //-> parse received buffer
		std::vector<std::string> _split_buffer(const std::string &buffer, const std::string &delimiter); //-> split string

		Client* _get_client(const int fd); //-> get client
		Client* _get_client(const std::string nickname); // -> get nickname
		bool _client_is_ready_to_login(const int fd);

		Channel* _get_channel(const std::string &channel_name); //-> get channel
		void _close_fds(); //-> close file descriptors

		void _add_channel(Channel *channel); // -> add a new channel to server channels

		std::string toupper(const std::string& str);
};

#endif // SERVER_HPP