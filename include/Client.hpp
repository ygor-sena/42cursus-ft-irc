/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:19:06 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/31 19:13:03 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <arpa/inet.h>	 //-> for inet_ntoa()
#include <fcntl.h>		 //-> for fcntl()
#include <netinet/in.h>	 //-> for sockaddr_in
#include <poll.h>		 //-> for poll()
#include <sys/socket.h>	 //-> for socket()
#include <sys/types.h>	 //-> for socket()
#include <unistd.h>		 //-> for close()
#include <algorithm>
#include <csignal>	//-> for signal()
#include <iostream>
#include <string>
#include <vector>  //-> for vector
#include "Replies.hpp"

//-------------------------------------------------------//
#define RED "\e[1;31m"	//-> for red color
#define WHI "\e[0;37m"	//-> for white color
#define GRE "\e[1;32m"	//-> for green color
#define YEL "\e[1;33m"	//-> for yellow color
//-------------------------------------------------------//

class Client  //-> class for client
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

  private:
	int _fd;				 //-> client file descriptor
	bool _is_logged;		 //-> boolean for login
	bool _is_authenticated;	 //-> boolean for authentication
	bool _is_operator;		 //-> boolean for channel operator
	std::string _buffer;	 //-> client buffer
	std::string _ip_addr;	 //-> client ip address
	std::string _nickname;	 //-> client nickname
	std::string _username;	 //-> client username
	std::string _password;	 //-> client password
	std::vector<std::string> _channels_invited;	 //-> vector of channels invited
};

#endif	// CLIENT_HPP