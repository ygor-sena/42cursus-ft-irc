/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:26:55 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/31 21:06:46 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{
	_hostname = "ft_irc.42.fr";
	_reply_code = 0;
	_port = 0;
	_server_fdsocket = -1;
	_password = "";
	memset(&_server_addr, 0, sizeof(_server_addr));
	_fds = std::vector<struct pollfd>();
	_clients = std::vector<Client*>();
	_channels = std::vector<Channel*>();
}

/*
** -------------------------- PARAMETER CONSTRUCTOR ---------------------------
*/
Server::Server(std::string password, std::vector<Client*> clients,
			   std::vector<Channel*> channels)
{
	_hostname = "ft_irc.42.fr";
	_reply_code = 0;
	_port = 0;
	_server_fdsocket = -1;
	_password = password;
	memset(&_server_addr, 0, sizeof(_server_addr));
	_fds = std::vector<struct pollfd>();
	this->_clients = clients;
	this->_channels = channels;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server()
{
	for (std::vector<Client*>::iterator it = _clients.begin();
		 it != _clients.end();
		 ++it)
	{
		delete *it;
	}
	_clients.clear();

	for (std::vector<Channel*>::iterator it = _channels.begin();
		 it != _channels.end();
		 ++it)
	{
		delete *it;
	}
	_channels.clear();
}

/*
** ------------------------------- GETTERS --------------------------------
*/

/**
 * @brief Get the hostname of the server.
 *
 * This function returns the hostname of the server.
 *
 * @return The hostname of the server.
 */
std::string Server::_get_hostname()
{
	return _hostname;
}

/**
 * @brief Retrieves the client associated with the given file descriptor.
 *
 * This method retrieves the client object associated with the specified file
 * descriptor from the vector of active clients. It iterates through the list of
 * clients and returns the client object that matches the provided file
 * descriptor.
 *
 * @param fd The file descriptor associated with the client to retrieve.
 * @return The client object associated with the specified file descriptor.
 */
Client* Server::_get_client(const int fd)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->get_fd() == fd)
		{
			return _clients[i];
		}
	}
	return NULL;
}

/**
 * @brief Retrieves a client object based on the given nickname.
 *
 * This function searches for a client object in the list of clients
 * based on the provided nickname. If a client with the given nickname
 * is found, a pointer to that client is returned. If no client is found,
 * a null pointer is returned.
 *
 * @param nickname The nickname of the client to retrieve.
 * @return A pointer to the client object if found, otherwise a null pointer.
 */
Client* Server::_get_client(const std::string nickname)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->get_nickname() == nickname)
		{
			return _clients[i];
		}
	}
	return NULL;
}

/**
 * @brief Retrieves a channel object based on the given channel name.
 *
 * This function searches for a channel object in the list of channels
 * based on the provided channel name. If a channel with the given name
 * is found, a pointer to that channel is returned. If no channel is found,
 * a null pointer is returned.
 *
 * @param channel_name The name of the channel to retrieve.
 * @return A pointer to the channel object if found, otherwise a null pointer.
 */
Channel* Server::_get_channel(const std::string& channel_name)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i]->get_name() == channel_name)
		{
			return _channels[i];
		}
	}
	return NULL;
}

/**
 * @brief Checks if the client is in any channel.
 *
 * This function checks if the client is in any channel.
 *
 * @param fd The file descriptor associated with the client.
 * @return True if the client is in any channel, false otherwise.
 */
int Server::get_reply_code(void) { return _reply_code; }

/*
** ------------------------------- SETTERS --------------------------------
*/

/**
 * @brief Sets up the server socket for the server.
 *
 * This function creates a socket, sets the socket options, sets the socket to
 * non-blocking mode, binds the socket to the specified address and port, and
 * starts listening for incoming connections. The server socket is added to the
 * list of file descriptors for polling.
 *
 * * This method initializes the server by performing the following steps:
 * 1. Creates a server socket.
 * 2. Sets the server address and port.
 * 3. Sets socket options such as SO_REUSEADDR and O_NONBLOCK.
 * 4. Binds the server socket to the specified address and port.
 * 5. Starts listening for incoming connections.
 * 6. Adds the server socket to the pollfd vector for polling.
 * 7. Waits for incoming connections and processes data received from clients.
 *
 * @throws std::runtime_error if any of the socket operations fail.
 */
void Server::_set_server_socket()
{
	struct pollfd new_poll;
	struct sockaddr_in server_addr;

	int enable = 1;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(_port);
	_server_fdsocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fdsocket == -1)
		throw(std::runtime_error("failed to create socket"));
	if (setsockopt(_server_fdsocket,
				   SOL_SOCKET,
				   SO_REUSEADDR,
				   &enable,
				   sizeof(enable)) == -1)
		throw(std::runtime_error(
			"failed to set option (SO_REUSEADDR) on socket"));
	if (fcntl(_server_fdsocket, F_SETFL, O_NONBLOCK) == -1)
		throw(
			std::runtime_error("failed to set option (O_NONBLOCK) on socket"));
	if (bind(_server_fdsocket,
			 (struct sockaddr*)&server_addr,
			 sizeof(server_addr)) == -1)
		throw(std::runtime_error("failed to bind socket"));
	if (listen(_server_fdsocket, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() failed"));
	new_poll.fd = _server_fdsocket;
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	_fds.push_back(new_poll);
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/**
 * @brief Initializes the server.
 *
 * This method initializes the server by setting the server port and password,
 * adding the server signal, creating the server socket, and starting the server
 * loop.
 *
 * @param port The server port.
 * @param password The server password.
 *
 * @throws std::invalid_argument if the port is invalid.
 * @throws std::runtime_error if an error occurs during server initialization.
 */
void Server::init(const std::string& port, const std::string& password)
{
	_is_valid_port(port);

	_port = std::atoi(port.c_str());
	_password = password;

	try
	{
		_add_server_signal();
		_set_server_socket();

		std::cout << "Waiting to accept a connection...\n";

		_server_loop();
		_close_fds();
	}
	catch (const std::exception& e)
	{
		_close_fds();
		std::cerr << e.what() << '\n';
	}
}

/**
 * @brief Starts the server loop.
 *
 * This method initiates the server loop, continuously waiting for incoming
 * connections and processing data received from clients. It utilizes the poll
 * system call to efficiently monitor file descriptors for events.
 *
 * The server loop continues to run until a termination signal is received.
 *
 * @throws std::runtime_error if an error occurs during the polling process.
 */
void Server::_server_loop()
{
	while (Server::_signal == false)
	{
		if ((poll(&_fds[0], _fds.size(), -1) == -1) && Server::_signal == false)
			throw(std::runtime_error("poll() faild"));
		for (size_t i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _server_fdsocket)
					_accept_new_client();
				else
					_receive_new_data(_fds[i].fd);
			}
		}
	}
}

/**
 * @brief Accepts a new client connection.
 *
 * This method is called when a new client connection is detected by the server.
 * It accepts the incoming connection, sets it to non-blocking mode, and adds
 * the client socket to the list of active clients for polling. Additionally, it
 * retrieves the IP address of the client and associates it with the client
 * object.
 *
 * @note If the acceptance of the new client connection fails or setting the
 * socket to non-blocking mode fails, appropriate error messages are printed to
 * the standard output, and the function returns.
 */
void Server::_accept_new_client()
{
	Client cli;
	struct pollfd new_poll;
	struct sockaddr_in cli_addr;

	memset(&cli_addr, 0, sizeof(cli_addr));
	socklen_t len = sizeof(cli_addr);
	int incofd = accept(_server_fdsocket, (sockaddr*)&(cli_addr), &len);
	if (incofd == -1)
	{
		std::cout << "accept() failed" << std::endl;
		return;
	}
	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cout << "fcntl() failed" << std::endl;
		close(incofd);
		return;
	}
	new_poll.fd = incofd;
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	cli.set_fd(incofd);
	cli.set_ip_add(inet_ntoa((cli_addr.sin_addr)));
	_clients.push_back(new Client(cli));
	_fds.push_back(new_poll);
	std::cout << GRE << "Client <" << incofd << "> Connected" << WHI
			  << std::endl;
}

/**
 * @brief Receives new data from the client.
 *
 * This method is called when new data is received from the client. It reads the
 * data from the client socket, processes the command, and executes the
 * corresponding action. If the client disconnects during the data transfer, the
 * client is removed from the list of active clients.
 *
 * @param fd The file descriptor associated with the client that sent the data.
 */
void Server::_receive_new_data(const int fd)
{
	char buffer[1024];
	std::memset(buffer, 0, sizeof(buffer));

	Client* cli = _get_client(fd);
	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0)
	{
		std::cout << RED << "Client <" << fd << "> Disconnected" << WHI
				  << std::endl;
		_clear_client(fd);
	}
	else
	{
		cli->set_buffer(buffer);
		if (cli->get_buffer().find_first_of(LINE_FEED) == std::string::npos)
			return;
		_execute_command(cli->get_buffer(), fd);
	}
}

/**
 * @brief Array of command handlers for the Server class.
 *
 * This array maps command names to their corresponding handler functions.
 * Each element of the array is a struct containing the command name and a
 * pointer to the handler function.
 *
 * The command handlers are used to process incoming commands from clients.
 * When a command is received, the server looks up the corresponding handler
 * function in this array and calls it.
 *
 * The command list is defined as a static member of the Server class.
 *
 * @see Server::_command_list_size
 * @see Server::_handler_client_join
 * @see Server::_handler_client_quit
 * @see Server::_handler_client_part
 * @see Server::_handler_client_mode
 * @see Server::_handler_client_kick
 * @see Server::_handler_client_topic
 * @see Server::_handler_client_nickname
 * @see Server::_handler_client_username
 * @see Server::_handler_client_password
 * @see Server::_handler_client_invite
 * @see Server::_handler_client_privmsg
 * @see Server::_handler_bot_marvin
 * @see Server::_handler_bot_time
 * @see Server::_handler_bot_whois
 * @see Server::_handler_bot_whoami
 * @see Server::_handler_bot_quote
 */
const Server::command_handler Server::_command_list[_command_list_size] = {
	{"JOIN", &Server::_handler_client_join},
	{"QUIT", &Server::_handler_client_quit},
	{"PART", &Server::_handler_client_part},
	{"MODE", &Server::_handler_client_mode},
	{"KICK", &Server::_handler_client_kick},
	{"TOPIC", &Server::_handler_client_topic},
	{"NICK", &Server::_handler_client_nickname},
	{"USER", &Server::_handler_client_username},
	{"PASS", &Server::_handler_client_password},
	{"INVITE", &Server::_handler_client_invite},
	{"PRIVMSG", &Server::_handler_client_privmsg},
	{"!MARVIN", &Server::_handler_bot_marvin},
	{"!TIME", &Server::_handler_bot_time},
	{"!WHOIS", &Server::_handler_bot_whois},
	{"!WHOAMI", &Server::_handler_bot_whoami},
	{"!QUOTE", &Server::_handler_bot_quote},
};

/**
 * @brief Executes a command received from a client.
 *
 * This function takes a buffer containing a command received from a client and
 * executes it. The buffer is first cleansed by removing any carriage return and
 * line feed characters. Then, the buffer is split into a vector of strings
 * using a delimiter. The first element of the vector is considered the command,
 * which is converted to uppercase. The second element of the vector is
 * considered the parameters for the command. The function then iterates through
 * a command list and checks if the command matches any of the commands in the
 * list. If a match is found, the corresponding command handler is called with
 * the parameters and the file descriptor of the client.
 *
 * @param buffer The buffer containing the command received from the client.
 * @param fd The file descriptor of the client.
 */
void Server::_execute_command(const std::string buffer, const int fd)
{
	if (buffer.empty())
		return;
	std::string clean_buffer = _cleanse_buffer(buffer, CRLF);
	std::vector<std::string> splitted_buffer =
		_split_buffer(clean_buffer, DELIMITER);
	if (splitted_buffer.empty())
		return;
	std::string command = toupper(splitted_buffer[0]);
	std::string parameters = splitted_buffer[1];

	for (size_t i = 0; i < _command_list_size; i++)
	{
		if (command == _command_list[i].command)
		{
			(this->*_command_list[i].handler)(parameters, fd);
			break;
		}
	}
}

/*
** --------------------------- PARSER COMMANDS --------------------------------
*/

/**
 * @brief Splits a string into tokens using a delimiter.
 *
 * This function takes a string and a delimiter as input and splits the string
 * into tokens based on the delimiter. The first token is stored as the command,
 * and the remaining part of the string is stored as the parameters. The tokens
 * are then returned as a vector of strings.
 *
 * @param buffer The string to be split.
 * @param delimiter The delimiter used to split the string.
 * @return std::vector<std::string> The vector of tokens obtained after
 * splitting the string.
 */
std::vector<std::string> Server::_split_buffer(const std::string& buffer,
											   const std::string& delimiter)
{
	std::string command;
	std::string parameters;
	std::vector<std::string> tokens;
	std::istringstream iss(buffer);

	iss >> command;
	tokens.push_back(command);

	std::getline(iss >> std::ws, parameters);
	parameters.erase(0, parameters.find_first_not_of(delimiter));
	tokens.push_back(parameters);

	return tokens;
}

/**
 * @brief Cleanses the given buffer by removing specified characters.
 *
 * This function takes a buffer string and removes all the characters specified
 * in the 'chars_to_remove' string. It returns the cleansed buffer string.
 *
 * @param buffer The buffer string to be cleansed.
 * @param chars_to_remove The string containing characters to be removed from
 * the buffer.
 * @return The cleansed buffer string.
 *  @note This function assumes that `original_str` is not empty and contains at
 * least one character to be removed. If `chars_to_remove` is empty, the
 * function returns the original string without any changes.
 */
std::string Server::_cleanse_buffer(const std::string& buffer,
									const std::string& chars_to_remove)
{
	std::string clean_buffer;

	size_t pos = buffer.find_first_of(chars_to_remove);
	if (pos != std::string::npos)
		clean_buffer = buffer.substr(0, pos);
	else
		clean_buffer = buffer;
	return clean_buffer;
}

/*
** ----------------------------- CLEAR FUCTIONS -------------------------------
*/

/**
 * @brief Closes all file descriptors associated with the server and its
 * clients.
 *
 * This function closes all the file descriptors associated with the server and
 * its clients. It iterates through the clients vector and closes each client's
 * file descriptor. It also closes the server's file descriptor if it is valid.
 *
 * @note This function does not handle any exceptions that may occur during the
 * closing of file descriptors.
 *
 * @param None
 * @return None
 *
 * @see Server::_clients
 * @see Server::_server_fdsocket
 */
void Server::_close_fds()
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		std::cout << RED << "Client <" << _clients[i]->get_fd()
				  << "> Disconnected" << WHI << std::endl;
		close(_clients[i]->get_fd());
	}
	if (_server_fdsocket != -1)
	{
		std::cout << RED << "Server <" << _server_fdsocket << "> Disconnected"
				  << WHI << std::endl;
		close(_server_fdsocket);
	}
}

/**
 * @brief Removes a client from the server.
 *
 * This function removes a client from the server by searching for the client
 * with the given file descriptor (fd) in the vector of clients (_clients). If a
 * matching client is found, it is deleted, and the corresponding element in the
 * vector is set to NULL. Finally, the element is erased from the vector.
 *
 * @param fd The file descriptor of the client to be removed.
 */
void Server::_remove_client_from_server(const int fd)
{
	for (std::vector<Client*>::iterator it = _clients.begin();
		 it != _clients.end();
		 ++it)
	{
		if ((*it)->get_fd() == fd)
		{
			delete *it;
			*it = NULL;
			it = _clients.erase(it);
			break;
		}
	}
}

/**
 * @brief Removes a client file descriptor from the server's list of file
 * descriptors.
 *
 * This function removes the specified client file descriptor from the server's
 * internal list of file descriptors. It searches for the file descriptor in the
 * list and removes it if found. After removing the file descriptor, the
 * function also closes the file descriptor.
 *
 * @param fd The file descriptor to be removed.
 */
void Server::_remove_client_fd(const int fd)
{
	for (size_t i = 0; i < _fds.size(); i++)
	{
		if (_fds[i].fd == fd)
		{
			_fds.erase(_fds.begin() + i);
			break;
		}
	}
	close(fd);
}

/**
 * @brief Removes a client from all channels.
 *
 * This function removes the specified client from all channels in the server.
 *
 * @param fd The file descriptor of the client to be removed.
 */
void Server::_remove_client_from_channels(const int fd)
{
	Client* client = _get_client(fd);

	for (std::vector<Channel*>::iterator it = _channels.begin();
		 it != _channels.end();
		 ++it)
	{
		(*it)->remove_channel_client(client);
	}
}

/**
 * @brief Clears a client from the server.
 *
 * This function removes the client with the specified file descriptor (fd) from
 * the server's internal data structures. It removes the file descriptor from
 * the _fds vector and the corresponding client object from the _clients vector.
 * Finally, it closes the file descriptor.
 *
 * @param fd The file descriptor of the client to be cleared.
 */
void Server::_clear_client(const int fd)
{
	_remove_client_from_channels(fd);
	_remove_client_from_server(fd);
	_remove_client_fd(fd);
}

/*
** ----------------------------- SIGNAL FUCTIONS ------------------------------
*/

bool Server::_signal = false;
/**
 * @brief Signal handler function for the Server class.
 *
 * This function is called when a signal is received by the server. It sets the
 * static boolean
 * `_signal` to true, indicating that the server should stop.
 *
 * @param signum The signal number.
 */
void Server::_signal_handler(const int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::_signal = true;
}

/**
 * @brief Adds signal handlers for the server.
 *
 * This method sets up signal handlers for the server to catch specific signals,
 * such as SIGINT (generated by pressing Ctrl+C) and SIGQUIT (generated by
 * pressing Ctrl+\). When any of these signals are received, the corresponding
 * signal handler function Server::_signal_handler is invoked.
 */

void Server::_add_server_signal()
{
	signal(SIGINT, Server::_signal_handler);
	signal(SIGQUIT, Server::_signal_handler);
}

/*
** --------------------------- VALIDATIONS FUCTIONS ---------------------------
*/

/**
 * @brief Checks if the port is valid.
 *
 * This method verifies whether the provided port is valid by performing the
 * following checks:
 * 1. Ensures that the port consists only of numeric characters.
 * 2. Validates that the port number falls within the range of valid port
 * numbers (1024 - 65535).
 *
 * @param port The port to check represented as a string.
 * @throws std::invalid_argument if the port is invalid (contains non-numeric
 * characters or is out of range).
 */
void Server::_is_valid_port(const std::string& port)
{
	if (!(port.find_first_not_of("0123456789") == std::string::npos &&
		  std::atoi(port.c_str()) >= 1024 && std::atoi(port.c_str()) <= 65535))
	{
		throw std::invalid_argument(
			"Invalid port: either contains non-numeric characters or is out of "
			"range");
	}
}

/**
 * @brief Checks if the nickname is valid.
 *
 * This method verifies whether the provided nickname is valid by performing the
 * following checks:
 * 1. Ensures that the nickname is at least 6 characters long.
 * 2. Validates that the nickname contains only alphanumeric characters.
 *
 * @param nickname The nickname to check.
 * @return true if the nickname is valid, false otherwise.
 */
bool Server::_is_valid_nickname(const std::string& nickname)
{
	if (nickname.size() <= 3)
		return false;
	for (std::string::const_iterator it = nickname.begin();
		 it != nickname.end();
		 ++it)
	{
		if (!std::isalnum(*it))
			return false;
	}
	return true;
}

/**
 * @brief Checks if the nickname is already in use.
 *
 * This method checks if the provided nickname is already in use by another
 * client. It iterates through the list of active clients and compares the
 * provided nickname with the nicknames of other clients. If a match is found,
 * the method returns true, indicating that the nickname is in use.
 *
 * @param fd The file descriptor associated with the client.
 * @param username The nickname to check.
 * @return true if the nickname is in use, false otherwise.
 */
bool Server::_is_nickname_in_use(const int fd, const std::string& username)
{
	for (std::vector<Client*>::iterator it = _clients.begin();
		 it != _clients.end();
		 ++it)
	{
		if ((*it)->get_nickname() == username && (*it)->get_fd() != fd)
			return true;
	}
	return false;
}

/*
** ----------------------------- CLIENT FUCTIONS ------------------------------
*/

/**
 * @brief Retrieves a client object based on the given username.
 *
 * This function searches for a client object in the list of clients
 * based on the provided username. If a client with the given username
 * is found, a pointer to that client is returned. If no client is found,
 * a null pointer is returned.
 *
 * @param username The username of the client to retrieve.
 * @return A pointer to the client object if found, otherwise a null pointer.
 */
bool Server::_client_is_ready_to_login(const int fd)
{
	Client* client = _get_client(fd);

	if (!client->get_username().empty() && !client->get_nickname().empty() &&
		!client->get_is_logged())
		return true;
	return false;
}

/*
** ------------------------------ UTILS FUCTIONS ------------------------------
*/

/**
 * @brief Adds a new channel to the server.
 *
 * This function adds a new channel to the list of channels maintained by the
 * server. The channel object is created with the specified name and added to
 * the list of channels.
 *
 * @param channel The name of the channel to be added.
 */
void Server::_add_channel(Channel* channel) { _channels.push_back(channel); }

/**
 * @brief Sends a response to the client.
 *
 * This method sends a response to the client associated with the specified file
 * descriptor. It sends the response message to the client socket using the send
 * system call.
 *
 * @param fd The file descriptor associated with the client to send the
 * response.
 * @param response The response message to send to the client.
 */
void Server::_send_response(const int fd, const std::string& response)
{
	std::cout << "Response:\n" << response;
	if (send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Response send() failed" << std::endl;
}

/**
 * @brief Converts a string to uppercase.
 *
 * This method converts the provided string to uppercase by iterating through
 * each character in the string and converting it to its uppercase equivalent
 * using the std::toupper function.
 *
 * @param str The string to convert to uppercase.
 * @return The converted string in uppercase.
 */
std::string Server::toupper(const std::string& str)
{
	std::string result = str;
	for (size_t i = 0; i < result.length(); ++i)
	{
		result[i] = std::toupper(result[i]);
	}
	return result;
}

/*
** ---------------------------- CHANNEL FUCTIONS ------------------------------
*/

/**
 * @brief Checks if the client is in any channel.
 *
 * This function checks if the client is in any channel.
 *
 * @param fd The file descriptor associated with the client.
 * @return True if the client is in any channel, false otherwise.
 */
bool Server::_is_client_in_any_channel(const int fd)
{
	Client* client = this->_get_client(fd);
	for (std::vector<Channel*>::iterator it = _channels.begin();
		 it != _channels.end();
		 ++it)
	{
		if ((*it)->has_client(client))
		{
			return true;
		}
	}
	return false;
}
