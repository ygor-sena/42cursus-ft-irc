/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:23:47 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/31 22:07:49 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string> /**< For std::to_string */
# include <cstring> /**< For std::strlen */
# include <cstdlib> /**< For std::atoi */
# include <iostream> /**< For std::cout */
# include <vector> /**< For vector */
# include <sys/socket.h> /**< For socket() */
# include <sys/types.h> /**< For socket() */
# include <netinet/in.h> /**< For sockaddr_in */
# include <fcntl.h> /**< For fcntl() */
# include <unistd.h> /**< For close() */
# include <arpa/inet.h> /**< For inet_ntoa() */
# include <poll.h> /**< For poll() */
# include <csignal> /**< For signal() */
# include <sstream> /**< For std::stringstream */
# include <iomanip> /**< For std::setfill */
# include <map> /**< For map */

//-------------------------------------------------------//
# define RED "\e[1;31m" /**< For red color */
# define WHI "\e[0;37m" /**< For white color */
# define GRE "\e[1;32m" /**< For green color */
# define YEL "\e[1;33m" /**< For yellow color */
//-------------------------------------------------------//

# include "Client.hpp" /**< For client class */
# include "Replies.hpp" /**< For replies class */
# include "Channel.hpp" /**< For channel class */

# define SPACE " " /**< For space */
# define CRLF "\r\n" /**< For CRLF */
# define LINE_FEED "\n" /**< For line feed */
# define DELIMITER " \t" /**< For delimiter */

/**
 * @brief The Server class represents an IRC server.
 */
class Server
{
	public:
		/**
		 * @brief Default constructor for the Server class.
		 */
		Server();

		/**
		 * @brief Destructor for the Server class.
		 */
		~Server();

		/**
		 * @brief Constructor for the Server class used as a workaround for mockup tests.
		 * @param password The server password.
		 * @param _clients The vector of clients.
		 * @param _channels The vector of channels.
		 */
		Server(std::string password, std::vector<Client> _clients, std::vector<Channel> _channels);

		/**
		 * @brief Initializes the server.
		 * @param port The server port.
		 * @param password The server password.
		 */
		void init(const std::string &port, const std::string &password);

	private:
		int _port; /**< The server port. */
		int _server_fdsocket; /**< The server socket file descriptor. */
		std::string _password; /**< The server password. */
		std::vector<Client> _clients; /**< The vector of clients. */
		std::vector<struct pollfd> _fds; /**< The vector of pollfd. */
		struct sockaddr_in _server_addr; /**< The server address. */
		std::vector<Channel *> _channels; /**< The vector of channels. */

		int _reply_code; /**< This is a workaround to test the IRC commands without mocking up a client-server connection. */

		/**
		 * @brief Gets the reply code.
		 * @return The reply code.
		 */
		int get_reply_code(void);

		/**
		 * @brief Checks if the port is valid.
		 * @param port The port to check.
		 */
		void _is_valid_port(const std::string &port);

		/**
		 * @brief Checks if the nickname is valid.
		 * @param nickname The nickname to check.
		 * @return True if the nickname is valid, false otherwise.
		 */
		bool _is_valid_nickname(const std::string &nickname);

		/**
		 * @brief Checks if the nickname is already in use.
		 * @param fd The file descriptor of the client.
		 * @param nickname The nickname to check.
		 * @return True if the nickname is in use, false otherwise.
		 */
		bool _is_nickname_in_use(const int fd, const std::string &nickname);

		/**
		 * @brief Checks if the client is in any channel.
		 * @param fd The file descriptor of the client.
		 * @return True if the client is in any channel, false otherwise.
		 */
		bool _is_client_in_any_channel(const int fd);

		/**
		 * @brief Handles the client join command.
		 * @param buffer The command buffer.
		 * @param fd The file descriptor of the client.
		 */
		void _handler_client_join(const std::string &buffer, const int fd);

		/**
		 * @brief Handles the client quit command.
		 * @param buffer The command buffer.
		 * @param fd The file descriptor of the client.
		 */
		void _handler_client_quit(const std::string &buffer, const int fd);

		/**
 * @brief
		 * @param buffer The command buffer.
		 * @param fd The file descriptor of the client.
		 */
		void _handler_client_part(const std::string &buffer, const int fd);

		/**
		 * @brief
		 * @param buffer The command buffer.
		 * @param fd The file descriptor of the client.
		 */
		void _handler_client_mode(const std::string &buffer, const int fd);

		/**
		 * @brief
		 * @param buffer The command buffer.
		 * @param fd The file descriptor of the client.
		 */
		void _handler_client_kick(const std::string &buffer, const int fd);

		/**
		 * @brief
		 * @param buffer The command buffer.
		 * @param fd The file descriptor of the client.
		 */
		void _handler_client_topic(const std::string &buffer, const int fd);

		/**
		 * @brief
		 * @param buffer The command buffer.
		 * @param fd The file descriptor of the client.
		 */
		void _handler_client_invite(const std::string &buffer, const int fd);

		/**
		 * @brief
		 * @param buffer The command buffer.
		 * @param fd The file descriptor of the client.
		 */
		void _handler_client_privmsg(const std::string &buffer, const int fd);

		/**
		 * @brief Handles the client nickname command.
		 * @param nickname The nickname.
		 * @param fd The file descriptor of the client.
		 */
		void _handler_client_nickname(const std::string &nickname, const int fd);

		/**
		 * @brief
		 * @param username The username.
		 * @param fd The file descriptor of the client.
		 */
		void _handler_client_username(const std::string &username, const int fd);

		/**
		 * @brief
		 * @param password The password.
		 * @param fd The file descriptor of the client.
		 */
		void _handler_client_password(const std::string &password, const int fd);

		/**
		 * @brief
		 * @param buffer The command buffer.
		 * @param fd The file descriptor of the client.
		 */
		void _handler_bot_marvin(const std::string &buffer, int fd);

		/**
		 * @brief
		 * @param buffer The command buffer.
		 * @param fd The file descriptor of the client.
		 */
		void _handler_bot_time(const std::string &buffer, int fd);

		/**
		 * @brief
		 * @param buffer The command buffer.
		 * @param fd The file descriptor of the client.
		 */
		void _handler_bot_whois(const std::string &buffer, int fd);

		/**
		 * @brief
		 * @param buffer The command buffer.
		 * @param fd The file descriptor of the client.
		 */
		void _handler_bot_whoami(const std::string &buffer, int fd);

		/**
		 * @brief
		 * @param buffer The command buffer.
		 * @param fd The file descriptor of the client.
		 */
		void _handler_bot_quote(const std::string &buffer, int fd);

		/**
		 * @brief
		 * @param channel The channel.
		 * @param set True to set the mode, false to unset the mode.
		 */
		void _handler_invite_only_mode(Channel* channel, bool set);

		/**
		 * @brief Handles the topic restriction mode for a channel.
		 * @param channel The channel.
		 * @param set True to set the mode, false to unset the mode.
		 */
		void _handler_topic_restriction_mode(Channel* channel, bool set);

		/**
		 * @brief
		 * @param channel The channel.
		 * @param client The client.
		 * @param set True to set the mode, false to unset the mode.
		 */
		void _handle_operator_privileges_mode(Channel* channel, Client* client, bool set);

		/**
		 * @brief
		 * @param channel The channel.
		 * @param argument The argument.
		 * @param set True to set the mode, false to unset the mode.
		 */
		void _handle_password_mode(Channel* channel, const std::string &argument, bool set);

		/**
		 * @brief
		 * @param channel The channel.
		 * @param client The client.
		 * @param argument The argument.
		 * @param set True to set the mode, false to unset the mode.
		 * @param fd The file descriptor of the client.
		 */
		void _handle_limit_mode(Channel* channel, Client* client, const std::string &argument, bool set, const int fd);

		/**
		 * @brief Handles the mode flags for a channel.
		 * @param channel The channel.
		 * @param client The client.
		 * @param mode The mode flag.
		 * @param set True to set the mode, false to unset the mode.
		 * @param arg The argument.
		 * @param fd The file descriptor of the client.
		 * @return True if the mode flag was handled successfully, false otherwise.
		 */
		bool _handler_mode_flags(Channel* channel, Client* client, char mode, bool set, const std::string &arg, const int fd);

		/**
		 * @brief Parses the mode command.
		 * @param modes The mode string.
		 * @param channel The channel.
		 * @param client The client.
		 * @param arg The argument.
		 * @param fd The file descriptor of the client.
		 * @return True if the mode command was parsed successfully, false otherwise.
		 */
		bool _parse_mode_command(const std::string &modes, Channel* channel, Client* client, const std::string &arg, const int fd);

		static bool _signal; /**< Static boolean for signal. */
		static void _signal_handler(const int signum);

		/**
		 * @brief The server loop.
		 */
		void _server_loop();

		/**
		 * @brief Creates the server socket.
		 */
		void _set_server_socket();

		/**
		 * @brief Adds the server signal.
		 */
		void _add_server_signal();

		/**
		 * @brief Accepts a new client.
		 */
		void _accept_new_client();

		/**
		 * @brief Clears the client.
		 * @param fd The file descriptor of the client.
		 */
		void _clear_client(const int fd);

		/**
		 * @brief Receives new data from a client.
		 * @param fd The file descriptor of the client.
		 */
		void _receive_new_data(const int fd);

		/**
		 * @brief Sends a response to a client.
		 * @param fd The file descriptor of the client.
		 * @param response The response to send.
		 */
		void _send_response(const int fd, const std::string &response);

		struct command_handler
		{
			std::string command;
			void (Server::*handler)(const std::string &, const int);
		};

		static const int _command_list_size = 16; /**< The command list size. */
		static const command_handler _command_list[_command_list_size]; /**< The command list. */

		/**
		 * @brief Executes a command.
		 * @param buffer The command buffer.
		 * @param fd The file descriptor of the client.
		 */
		void _execute_command(const std::string buffer, const int fd);

		/**
		 * @brief Closes file descriptors.
		 */
		void _close_fds();

		/**
		 * @brief Cleanses the buffer.
		 * @param buffer The buffer to cleanse.
		 * @param chars_to_remove The characters to remove.
		 * @return The cleansed buffer.
		 */
		std::string _cleanse_buffer(const std::string &buffer, const std::string &chars_to_remove);

		/**
		 * @brief Splits a buffer into a vector of strings.
		 * @param buffer The buffer to split.
		 * @param delimiter The delimiter.
		 * @return The vector of strings.
		 */
		std::vector<std::string> _split_buffer(const std::string &buffer, const std::string &delimiter);

		/**
		 * @brief Gets a client by file descriptor.
		 * @param fd The file descriptor of the client.
		 * @return The client.
		 */
		Client* _get_client(const int fd);

		/**
		 * @brief Adds a channel to the server channels.
		 * @param channel The channel to add.
		 */
		void _add_channel(Channel *channel);

		/**
		 * @brief Checks if a client is ready to login.
		 * @param fd The file descriptor of the client.
		 * @return True if the client is ready to login, false otherwise.
		 */
		bool _client_is_ready_to_login(const int fd);

		/**
		 * @brief Gets a client by nickname.
		 * @param nickname The nickname.
		 * @return The client.
		 */
		Client* _get_client(const std::string nickname);

		/**
		 * @brief Gets a channel by name.
		 * @param channel_name The channel name.
		 * @return The channel.
		 */
		Channel* _get_channel(const std::string &channel_name);

		/**
		 * @brief Converts a string to uppercase.
		 * @param str The string to convert.
		 * @return The uppercase string.
		 */
		std::string toupper(const std::string& str);
};


#endif // SERVER_HPP
