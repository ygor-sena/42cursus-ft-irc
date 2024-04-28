/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:26:55 by gilmar            #+#    #+#             */
/*   Updated: 2024/04/28 17:09:58 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::_signal = false; //-> initialize the static boolean

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{
    _server_fdsocket = -1;
}

/*
** --------------------------------- METHODS ----------------------------------
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
void Server::_signal_handler(int signum)
{
    (void)signum;
    std::cout << std::endl << "Signal Received!" << std::endl;
    Server::_signal = true; //-> set the static boolean to true to stop the server
}

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
        std::cout << RED << "Client <" << _clients[i].GetFd() << "> Disconnected" << WHI << std::endl;
        close(_clients[i].GetFd());
    }
    if (_server_fdsocket != -1) {
        std::cout << RED << "Server <" << _server_fdsocket << "> Disconnected" << WHI << std::endl;
        close(_server_fdsocket);
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
    
	int enable = 1;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(_port);
	_server_fdsocket = socket(AF_INET, SOCK_STREAM, 0);
	if(_server_fdsocket == -1)
		throw(std::runtime_error("faild to create socket"));
	if(setsockopt(_server_fdsocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1)
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
    if (fcntl(_server_fdsocket, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	if (bind(_server_fdsocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		throw(std::runtime_error("faild to bind socket"));
	if (listen(_server_fdsocket, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() faild"));
	new_poll.fd = _server_fdsocket;
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	_fds.push_back(new_poll);
}

/**
 * @brief Starts the server loop.
 *
 * This method initiates the server loop, which continuously waits for incoming connections
 * and processes the data received from clients. It utilizes the poll system call to monitor
 * file descriptors for events and handle them appropriately.
 *
 * The server loop runs until the termination signal is received.
 *
 * @throws std::runtime_error if an error occurs during the polling process.
 */
void Server::_server_loop()
{
    while (Server::_signal == false)
    {
        if((poll(&_fds[0], _fds.size(), -1) == -1) && Server::_signal == false)
            throw(std::runtime_error("poll() faild"));
        for (size_t i = 0; i < _fds.size(); i++) {
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
    struct pollfd new_poll;
    struct sockaddr_in cli_addr;

    socklen_t len = sizeof(cli_addr);
	int incofd = accept(_server_fdsocket, (sockaddr *)&(cli_addr), &len);
	if (incofd == -1)
		{std::cout << "accept() failed" << std::endl; return;}
	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1)
		{std::cout << "fcntl() failed" << std::endl; return;}
	new_poll.fd = incofd;
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	cli.SetFd(incofd);
	cli.setIpAdd(inet_ntoa((cli_addr.sin_addr)));
	_clients.push_back(cli);
	_fds.push_back(new_poll);
	std::cout << GRE << "Client <" << incofd << "> Connected" << WHI << std::endl;
}

void Server::_receive_new_data(int fd)
{
    char buff[1024]; //-> buffer for the received data
    std::memset(buff, 0, sizeof(buff)); //-> clear the buffer


    // TODO: Obter o cliente que está armazenado na std::vector<Client> _clients através do file descriptor fd

    ssize_t bytes = recv(fd, buff, sizeof(buff) - 1 , 0); //-> receive the data
    if(bytes <= 0) { //-> check if the client disconnected
        std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
        _clear_client(fd); //-> clear the client
    }
    else { //-> print the received data
        buff[bytes] = '\0';
        std::cout << YEL << "Client <" << fd << "> Data: " << WHI << buff;

        
        //here you can add your code to process the received data: parse, check, authenticate, handle the command, etc...
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
void Server::_clear_client(int fd)
{
    // Remove the client from the pollfd
    for(size_t i = 0; i < _fds.size(); i++) {
        if (_fds[i].fd == fd){
            _fds.erase(_fds.begin() + i);
            break;
        }
    }
    // Remove the client from the vector of clients
    for(size_t i = 0; i < _clients.size(); i++) {
        if (_clients[i].GetFd() == fd) {
            _clients.erase(_clients.begin() + i); 
            break;
        }
    }
    close(fd);
}
