/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:23:47 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/01 19:29:07 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string> // Para std::to_string
#include <cstring>
#include <cstdlib> // para a função std::atoi
#include <iostream>
#include <vector> //-> for vector
#include <sys/socket.h> //-> for socket()
#include <sys/types.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal> //-> for signal()
#include <sstream> //-> for std::stringstream

#include "Client.hpp" //-> for client class
//-------------------------------------------------------//
#define RED "\e[1;31m" //-> for red color
#define WHI "\e[0;37m" //-> for white color
#define GRE "\e[1;32m" //-> for green color
#define YEL "\e[1;33m" //-> for yellow color
//-------------------------------------------------------//

class Server //-> class for server
{
    public:
        Server(); //-> constructor
        ~Server(); //-> destructor

        void init(const std::string &port, const std::string &password); //-> server initialization
        
    private:
        int _port; //-> server port
        int _server_fdsocket; //-> server socket file descriptor
        std::string _password; //-> server password
        std::vector<Client> _clients; //-> vector of clients
        std::vector<struct pollfd> _fds; //-> vector of pollfd
        struct sockaddr_in _server_addr; //-> server address

        void _is_valid_port(const std::string &port); //-> check if the port is valid
        
        static bool _signal; //-> static boolean for signal
        static void _signal_handler(const int signum); //-> signal handler

        void _set_server_socket(); //-> server socket creation
        void _add_server_signal(); //-> server signal creation
        void _accept_new_client(); //-> accept new client
        void _receive_new_data(const int fd); //-> receive data from a client
        void _clear_client(const int fd); //-> clear clients
        void _server_loop(); //-> server loop
        
        void _execute_command(const std::string &buffer, const int fd); //-> execute command
        
        std::string _cleanse_buffer(const std::string buffer); //-> parse received buffer
        

        Client& _get_client(const int fd); //-> get client
        
        void _close_fds(); //-> close file descriptors
};