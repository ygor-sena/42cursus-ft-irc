/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:26:55 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/19 19:33:36 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{
    _server_fdsocket = -1;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server()
{
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/**
 * @brief Initializes the server.
 *
 * This method initializes the server by setting the server port and password,
 * adding the server signal, creating the server socket, and starting the server loop.
 *
 * @param port The server port.
 * @param password The server password.
 * 
 * @throws std::invalid_argument if the port is invalid.
 * @throws std::runtime_error if an error occurs during server initialization.
 */
void Server::init(const std::string &port, const std::string &password)
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
    catch(const std::exception& e)
    {
        _close_fds();
        std::cerr << e.what() << '\n';
    }
}

/**
 * @brief Initializes the server.
 *
 * This method initializes the server by performing the following steps:
 * 1. Creates a server socket.
 * 2. Sets the server address and port.
 * 3. Sets socket options such as SO_REUSEADDR and O_NONBLOCK.
 * 4. Binds the server socket to the specified address and port.
 * 5. Starts listening for incoming connections.
 * 6. Adds the server socket to the pollfd vector for polling.
 * 7. Waits for incoming connections and processes data received from clients.
 *
 * @throws std::runtime_error if any of the initialization steps fail.
 */
void Server::_set_server_socket()
{
    struct pollfd new_poll;
    struct sockaddr_in server_addr;
    
	int enable = 1; //-> enable the socket option
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(_port); //-> set the port
	_server_fdsocket = socket(AF_INET, SOCK_STREAM, 0); //-> create the socket
	if(_server_fdsocket == -1)
		throw(std::runtime_error("faild to create socket"));
	if(setsockopt(_server_fdsocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1) //-> set the socket options | Enable is used to reuse the address
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
    if (fcntl(_server_fdsocket, F_SETFL, O_NONBLOCK) == -1) //-> set the socket to non-blocking mode
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	if (bind(_server_fdsocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) //-> bind the socket
		throw(std::runtime_error("faild to bind socket"));
	if (listen(_server_fdsocket, SOMAXCONN) == -1) //-> listen for incoming connections
		throw(std::runtime_error("listen() faild"));
	new_poll.fd = _server_fdsocket;
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	_fds.push_back(new_poll);
}

/**
 * @brief Starts the server loop.
 *
 * This method initiates the server loop, continuously waiting for incoming connections
 * and processing data received from clients. It utilizes the poll system call to efficiently
 * monitor file descriptors for events.
 *
 * The server loop continues to run until a termination signal is received.
 *
 * @throws std::runtime_error if an error occurs during the polling process.
 */
void Server::_server_loop()
{
    while (Server::_signal == false)
    {
        // Poll for events on file descriptors
        if((poll(&_fds[0], _fds.size(), -1) == -1) && Server::_signal == false)
            throw(std::runtime_error("poll() faild"));
        for (size_t i = 0; i < _fds.size(); i++) {
            // Check if the file descriptor has an event
            if (_fds[i].revents & POLLIN) {
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
 * This method is called when a new client connection is detected by the server. It accepts
 * the incoming connection, sets it to non-blocking mode, and adds the client socket to the
 * list of active clients for polling. Additionally, it retrieves the IP address of the client
 * and associates it with the client object.
 *
 * If the acceptance of the new client connection fails or setting the socket to non-blocking mode
 * fails, appropriate error messages are printed to the standard output, and the function returns.
 */
void Server::_accept_new_client()
{
	Client cli; //-> create a new client
    struct pollfd new_poll; //-> create a new pollfd
    struct sockaddr_in cli_addr;  //-> create a new sockaddr_in

    memset(&cli_addr, 0, sizeof(cli_addr));
    socklen_t len = sizeof(cli_addr);
	int incofd = accept(_server_fdsocket, (sockaddr *)&(cli_addr), &len); // -> accept the incoming connection
	if (incofd == -1)
		{std::cout << "accept() failed" << std::endl; return;}
	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket to non-blocking mode
		{std::cout << "fcntl() failed" << std::endl; return;}
	new_poll.fd = incofd; //-> set the file descriptor
	new_poll.events = POLLIN; //-> set the events
	new_poll.revents = 0;
	cli.set_fd(incofd); //-> set the file descriptor
	cli.set_ip_add(inet_ntoa((cli_addr.sin_addr))); //-> set the ip address
	_clients.push_back(cli);
	_fds.push_back(new_poll);
	std::cout << GRE << "Client <" << incofd << "> Connected" << WHI << std::endl;
}

void Server::_receive_new_data(const int fd)
{
    char buffer[1024]; //-> buffer for the received data
    std::memset(buffer, 0, sizeof(buffer)); //-> clear the buffer
    
    Client* cli = _get_client(fd); // -> get the client object associated with the file descriptor (fd)
    ssize_t bytes = recv(fd, buffer, sizeof(buffer) -1 , 0); //-> receive the data
    if(bytes <= 0) { //-> check if the client disconnected
        std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
        _clear_client(fd); //-> clear the client
    }
    else {
        cli->set_buffer(buffer); //-> set the buffer with the received data
        if (cli->get_buffer().find_first_of(LINE_FEED) == std::string::npos) //-> check if the data is complete
            return;
        _execute_command(cli->get_buffer(), fd); //-> execute the command
    }
}

/**
 * @brief List of commands and their corresponding handlers.
 *
 * This list contains the commands supported by the server and their corresponding handler functions.
 * Each entry in the list consists of a command string and a pointer to the handler function for that command.
 * The handler functions are responsible for processing the command and executing the corresponding action.
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
    {"PRIVMSG", &Server::_handler_client_privmsg}
};

/**
 * @brief Executes the command received from the client.
 *
 * This method processes the command received from the client and executes the corresponding action.
 * It parses the received buffer into individual commands and iterates through each command to execute
 * the corresponding action. The method is responsible for handling various commands such as authentication,
 * message sending, and other server operations.
 *
 * @param command The received buffer containing the command to be executed.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_execute_command(const std::string buffer, const int fd)
{
    if (buffer.empty())
		return ;
    std::string clean_buffer = _cleanse_buffer(buffer, CRLF);
    std::vector<std::string> splitted_buffer = _split_buffer(clean_buffer, DELIMITER);
    if (splitted_buffer.empty())
        return ;
    std::string command = toupper(splitted_buffer[0]);
    std::string parameters = splitted_buffer[1];
    
    for (size_t i = 0; i < _command_list_size; i++) {
        if (command == _command_list[i].command) {
            (this->*_command_list[i].handler)(parameters, fd);
            break;
        }
    }
}

/*
** ------------------------------- JOIN COMMAND --------------------------------
*/

/**
 * @brief Handles the JOIN command received from the client.
 *
 * This method processes the JOIN command received from the client and sends a response
 * to the client indicating that the client has joined the channel.
 *
 * @param buffer The buffer containing the JOIN command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_join(const std::string &buffer, const int fd)
{
    Client* client = _get_client(fd);

    std::string channel = buffer;
    
    _send_response(fd, RPL_TOPICIS(client->get_nickname(), channel, std::string("Welcome to the channel")));
}

/*
** ------------------------------- QUIT COMMAND --------------------------------
*/

/**
 * @brief Handles the QUIT command received from the client.
 *
 * This method processes the QUIT command received from the client and sends a response
 * to the client indicating that the client has quit the server.
 *
 * @param buffer The buffer containing the QUIT command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_quit(const std::string &buffer, const int fd)
{
    (void)buffer;
    _send_response(fd, RPL_QUITMESSAGE(_get_client(fd)->get_nickname()));
    _clear_client(fd);
}

/*
** ------------------------------- PART COMMAND --------------------------------
*/

/**
 * @brief Handles the PART command received from the client.
 *
 * This method processes the PART command received from the client and sends a response
 * to the client indicating that the client has left the channel.
 *
 * @param buffer The buffer containing the PART command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_part(const std::string &buffer, const int fd)
{
    (void)fd;
    (void)buffer;
    Client* client = _get_client(fd);

    _send_response(fd, RPL_PART(client->get_hostname(), "ft_trancendence", client->get_nickname()));
}

/*
** ------------------------------- MODE COMMAND --------------------------------
*/

/**
 * @brief Handles the MODE command received from the client.
 *
 * This method processes the MODE command received from the client and sends a response
 * to the client indicating the channel modes supported by the server.
 *
 * @param buffer The buffer containing the MODE command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_mode(const std::string &buffer, const int fd)
{
    (void)fd;
    (void)buffer;
    Client* client = _get_client(fd);

    _send_response(fd, RPL_CHANNELMODES(client->get_nickname(), "ft_trancendence", "+nt"));
}

/*
** ------------------------------- KICK COMMAND --------------------------------
*/

/**
 * @brief Handles the KICK command received from the client.
 *
 * This method processes the KICK command received from the client and sends a response
 * to the client indicating that the client has been kicked from the channel.
 *
 * @param buffer The buffer containing the KICK command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_kick(const std::string &buffer, const int fd)
{
    (void)fd;
    (void)buffer;
    Client* client = _get_client(fd);

    _send_response(fd, RPL_KICK(client->get_nickname(), "ft_trancendence", "gilmar", "Ygor", "You have been kicked"));
}

/*
** ------------------------------- TOPIC COMMAND --------------------------------
*/

/**
 * @brief Handles the TOPIC command received from the client.
 *
 * This method processes the TOPIC command received from the client and sends a response
 * to the client indicating the topic of the channel.
 *
 * @param buffer The buffer containing the TOPIC command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_topic(const std::string &buffer, const int fd)
{
    (void)fd;
    (void)buffer;
    Client* client = _get_client(fd);

    _send_response(fd, RPL_TOPICIS(client->get_nickname(), "ft_trancendence", "Welcome to the channel"));
}

/*
** ------------------------------- INVITE COMMAND --------------------------------
*/

/**
 * @brief Handles the INVITE command received from the client.
 *
 * This method processes the INVITE command received from the client and sends a response
 * to the client indicating that the client has been invited to the channel.
 *
 * @param buffer The buffer containing the INVITE command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_invite(const std::string &buffer, const int fd)
{
    (void)fd;
    (void)buffer;
    Client* client = _get_client(fd);

    _send_response(fd, RPL_INVITING(client->get_hostname(), "ft_trancendence", "Gilmar", "Ygor"));
}

/*
** ------------------------------- INVITE COMMAND --------------------------------
*/

/**
 * @brief Handles the PRIVMSG command received from the client.
 *
 * This method processes the PRIVMSG command received from the client and sends a response
 * to the client indicating that the message has been sent.
 *
 * @param buffer The buffer containing the PRIVMSG command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_privmsg(const std::string &buffer, const int fd)
{
    (void)fd;
    (void)buffer;
    Client* client = _get_client(fd);

    _send_response(fd, RPL_PRIVMSG(client->get_hostname(), "ft_trancendence", "Hello, Carlos!"));
}

/*
** ------------------------------- NICK COMMAND --------------------------------
*/

/**
 * @brief Handles the NICK command received from the client.
 *
 * This method processes the NICK command received from the client and sends a response
 * to the client indicating that the client's nickname has been changed.
 *
 * @param nickname The new nickname to be assigned to the client.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_nickname(const std::string &nickname, const int fd)
{
    Client* client = _get_client(fd);

    if (nickname.empty() || nickname.size() < 5)
        _send_response(fd, ERR_NOTENOUGHPARAM(std::string("*")));
    else if (!_is_valid_nickname(nickname))
        _send_response(fd, ERR_ERRONEUSNICK(client->get_nickname()));
    else if (_is_nickname_in_use(fd, nickname))
        _send_response(fd, ERR_NICKINUSE(client->get_nickname()));
    else if (!client->get_is_registered())
        _send_response(fd, ERR_NOTREGISTERED(std::string("*")));
    else
    {
        client->set_nickname(nickname);
        if (_client_is_ready_to_login(fd))
            client->set_is_logged(fd);
    }
}

/*
** ------------------------------- USERNAME COMMAND --------------------------------
*/

/**
 * @brief Handles the USERNAME command received from the client.
 *
 * This method processes the USERNAME command received from the client and sends a response
 * to the client indicating that the client's username has been set.
 *
 * @param username The username to be assigned to the client.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_username(const std::string &username, const int fd)
{
    Client* client = _get_client(fd);

    if (username.empty() || username.size() < 5)
        _send_response(fd, ERR_NOTENOUGHPARAM(std::string("*")));
    else if (!client->get_is_registered())
        _send_response(fd, ERR_NOTREGISTERED(std::string("*")));
    else if (!client->get_username().empty())
        _send_response(fd, ERR_ALREADYREGISTERED(std::string("*")));
    else
    {
        client->set_username(username);
        if (_client_is_ready_to_login(fd))
            client->set_is_logged(fd);
    }
}

/*
** ------------------------------- PASSWORD COMMAND --------------------------------
*/

/**
 * @brief Handles the PASSWORD command received from the client.
 *
 * This method processes the PASSWORD command received from the client and sends a response
 * to the client indicating that the client's password has been set.
 *
 * @param password The password to be assigned to the client.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_password(const std::string &password, const int fd)
{
    Client* client = _get_client(fd);

    if (password.empty()) //-> Isso é o suficiente?
        _send_response(fd, ERR_NOTENOUGHPARAM(std::string("*")));
    else if (client->get_is_registered())
        _send_response(fd, ERR_ALREADYREGISTERED(std::string("*")));
    else if (_password != password)
        _send_response(fd, ERR_INCORPASS(std::string("*")));
    else
    {
        client->set_is_registered(true);
    }
}

/*
** ------------------------------- PARSER COMMAND --------------------------------
*/

/**
 * @brief Splits a buffer into tokens using a specified delimiter.
 * 
 * This method splits the original string into tokens using the specified delimiter.
 * It searches for the delimiter in the original string and splits the string into tokens
 * based on the delimiter. The tokens are stored in a vector and returned.
 * 
 * @param buffer The original string to be split.
 * @param delimiter The delimiter used to split the string.
 * 
 * @return A vector containing the tokens obtained by splitting the original string.
 */
std::vector<std::string> Server::_split_buffer(const std::string &buffer, const std::string &delimiter)
{
    std::vector<std::string> tokens;

    std::string command = buffer.substr(0, buffer.find_first_of(delimiter));
    std::string parameters = buffer.substr(buffer.find_first_of(delimiter) + 1);

    tokens.push_back(command);
    tokens.push_back(parameters);

    return tokens;
}

/**
 * @brief Cleanses a buffer by removing specified characters.
 *
 * This method removes specified characters from the original string and returns the cleaned string.
 * The characters to be removed are specified by the `chars_to_remove` parameter.
 * 
 * The function searches for the first occurrence of any character specified in `chars_to_remove`
 * in the original string `original_str`, and truncates the string up to that point, excluding the found character.
 * 
 * @param buffer The original string to be cleansed.
 * @param chars_to_remove A string containing the characters to be removed.
 * @return A string containing the original string with specified characters removed.
 * 
 * @note This function assumes that `original_str` is not empty and contains at least one character to be removed.
 *       If `chars_to_remove` is empty, the function returns the original string without any changes.
 */
std::string Server::_cleanse_buffer(const std::string &buffer, const std::string &chars_to_remove)
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
** ------------------------------- CLEAR FUCTIONS --------------------------------
*/

/**
 * @brief Closes all file descriptors associated with the server.
 *
 * This method closes all file descriptors associated with the server, including client connections
 * and the server socket itself. It iterates through the list of client connections, closing each
 * client socket individually. After that, it closes the server socket if it is valid.
 *
 * Upon closing each client connection and the server socket, it prints a message indicating the
 * disconnection to the standard output.
 */
void Server::_close_fds()
{
    //-> close all the clients
    for(size_t i = 0; i < _clients.size(); i++) {
        std::cout << RED << "Client <" << _clients[i].get_fd() << "> Disconnected" << WHI << std::endl;
        close(_clients[i].get_fd());
    }
    if (_server_fdsocket != -1) {
        std::cout << RED << "Server <" << _server_fdsocket << "> Disconnected" << WHI << std::endl;
        close(_server_fdsocket);
    }
}

/**
 * @brief Clears the client associated with the given file descriptor.
 *
 * This method removes the client with the specified file descriptor from the list of file descriptors
 * being polled (_fds) and from the vector of active clients (_clients). Additionally, it closes the
 * client socket.
 *
 * @param fd The file descriptor associated with the client to be cleared.
 */
void Server::_clear_client(const int fd)
{
    for(size_t i = 0; i < _fds.size(); i++) { //-> Remove the client from the pollfd
        if (_fds[i].fd == fd){
            _fds.erase(_fds.begin() + i);
            break;
        }
    }
    for(size_t i = 0; i < _clients.size(); i++) { //-> Remove the client from the vector of clients
        if (_clients[i].get_fd() == fd) {
            _clients.erase(_clients.begin() + i); 
            break;
        }
    }
    close(fd);
}

/*
** ------------------------------- SIGNAL FUCTIONS --------------------------------
*/

/**
 * @brief Signal handler function for the server.
 *
 * This function is called when a signal is received by the server process. It handles the signal
 * specified by the `signum` parameter. In this implementation, the function simply prints a message
 * indicating that a signal has been received and sets the static boolean variable `_signal` to true.
 * This variable is used to control the server loop and stop the server gracefully.
 *
 * @param signum The signal number that triggered the signal handler.
 */
bool Server::_signal = false; //-> initialize the static boolean
void Server::_signal_handler(const int signum)
{
    (void)signum;
    std::cout << std::endl << "Signal Received!" << std::endl;
    Server::_signal = true; //-> set the static boolean to true to stop the server
}

/**
 * @brief Adds signal handlers for the server.
 *
 * This method sets up signal handlers for the server to catch specific signals, such as SIGINT
 * (generated by pressing Ctrl+C) and SIGQUIT (generated by pressing Ctrl+\). When any of these
 * signals are received, the corresponding signal handler function Server::_signal_handler is invoked.
 */
void Server::_add_server_signal()
{
    signal(SIGINT, Server::_signal_handler); //-> catch the signal (ctrl + c)
    signal(SIGQUIT, Server::_signal_handler); //-> catch the signal (ctrl + \)
}

/*
** ------------------------------- VALIDATIONS FUCTIONS --------------------------------
*/

/**
 * @brief Checks if the port is valid.
 *
 * This method verifies whether the provided port is valid by performing the following checks:
 * 1. Ensures that the port consists only of numeric characters.
 * 2. Validates that the port number falls within the range of valid port numbers (1024 - 65535).
 *
 * @param port The port to check represented as a string.
 * @throws std::invalid_argument if the port is invalid (contains non-numeric characters or is out of range).
 */
void Server::_is_valid_port(const std::string &port)
{
    if (!(port.find_first_not_of("0123456789") == std::string::npos && \
        std::atoi(port.c_str()) >= 1024 && std::atoi(port.c_str()) <= 65535))
    {
        throw std::invalid_argument("Invalid port: either contains non-numeric characters or is out of range");
    }
}

/**
 * @brief Checks if the nickname is valid.
 *
 * This method verifies whether the provided nickname is valid by performing the following checks:
 * 1. Ensures that the nickname is at least 6 characters long.
 * 2. Validates that the nickname contains only alphanumeric characters.
 *
 * @param nickname The nickname to check.
 * @return true if the nickname is valid, false otherwise.
 */
bool Server::_is_valid_nickname(const std::string &nickname)
{
    if (nickname.size() <= 5)
        return false;
    for (std::string::const_iterator it = nickname.begin(); it != nickname.end(); ++it) {
        if (!std::isalnum(*it))
            return false;
    }
    return true;
}

/**
 * @brief Checks if the nickname is already in use.
 *
 * This method checks if the provided nickname is already in use by another client. It iterates
 * through the list of active clients and compares the provided nickname with the nicknames of
 * other clients. If a match is found, the method returns true, indicating that the nickname is in use.
 *
 * @param fd The file descriptor associated with the client.
 * @param username The nickname to check.
 * @return true if the nickname is in use, false otherwise.
 */
bool Server::_is_nickname_in_use(const int fd, const std::string &username)
{
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->get_nickname() == username && it->get_fd() != fd)
            return true;
    }
    return false;
}

/*
** ------------------------------- CLIENT FUCTIONS --------------------------------
*/

/**
 * @brief Retrieves the client associated with the given file descriptor.
 *
 * This method retrieves the client object associated with the specified file descriptor from the
 * vector of active clients. It iterates through the list of clients and returns the client object
 * that matches the provided file descriptor.
 *
 * @param fd The file descriptor associated with the client to retrieve.
 * @return The client object associated with the specified file descriptor.
 */
Client* Server::_get_client(const int fd)
{
    for (size_t i = 0; i < _clients.size(); i++) {
        if (_clients[i].get_fd() == fd) {
            return &_clients[i];
        }
    }
    throw std::invalid_argument("Client not found");
}

bool Server::_client_is_ready_to_login(const int fd)
{
    Client *client = _get_client(fd);
    
    if (!client->get_username().empty() && !client->get_nickname().empty() && !client->get_is_logged())
        return true;
    return false;
}

/*
** ------------------------------- UTILS FUCTIONS --------------------------------
*/

/**
 * @brief Sends a response to the client.
 * 
 * This method sends a response to the client associated with the specified file descriptor.
 * It sends the response message to the client socket using the send system call.
 * 
 * @param fd The file descriptor associated with the client to send the response.
 * @param response The response message to send to the client.
 */
void Server::_send_response(const int fd, const std::string &response)
{
	std::cout << "Response:\n" << response;
	if(send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Response send() faild" << std::endl;
}

/**
 * @brief Converts a string to uppercase.
 *
 * This method converts the provided string to uppercase by iterating through each character
 * in the string and converting it to its uppercase equivalent using the std::toupper function.
 *
 * @param str The string to convert to uppercase.
 * @return The converted string in uppercase.
 */
std::string Server::toupper(const std::string& str)
{
    std::string result = str;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = std::toupper(result[i]);
    }
    return result;
}