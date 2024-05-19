/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:23:47 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/19 18:55:27 by gilmar           ###   ########.fr       */
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
#include <iomanip>
#include <map> //-> for map
#include <functional> //-> for std::function

//-------------------------------------------------------//
#define RED "\e[1;31m" //-> for red color
#define WHI "\e[0;37m" //-> for white color
#define GRE "\e[1;32m" //-> for green color
#define YEL "\e[1;33m" //-> for yellow color
//-------------------------------------------------------//

#include "Replies.hpp"
#include "Client.hpp" //-> for client class

#define CRLF "\r\n"
#define LINE_FEED "\n"
#define DELIMITER " \t"

class Server
{
    public:
        Server();
        ~Server();

        void init(const std::string &port, const std::string &password);
        
    private:
        int _port; //-> server port
        int _server_fdsocket; //-> server socket file descriptor
        std::string _password; //-> server password
        std::vector<Client> _clients; //-> vector of clients
        std::vector<struct pollfd> _fds; //-> vector of pollfd
        struct sockaddr_in _server_addr; //-> server address

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

        void _send_response(const int fd, const std::string &response); //-> send response to client
        void _set_server_socket(); //-> server socket creation
        void _add_server_signal(); //-> server signal creation
        void _accept_new_client(); //-> accept new client
        void _receive_new_data(const int fd); //-> receive data from a client
        void _clear_client(const int fd); //-> clear clients
        void _server_loop(); //-> server loop
        
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
        bool _client_is_ready_to_login(const int fd);

        void _close_fds(); //-> close file descriptors
};