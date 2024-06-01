/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:19:06 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/31 21:52:47 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream> /**< for cout */
# include <vector> /**< for vector */
# include <sys/socket.h> /**< for socket() */
# include <sys/types.h> /**< for socket() */
# include <netinet/in.h> /**< for sockaddr_in */
# include <fcntl.h> /**< for fcntl() */
# include <unistd.h> /**< for close() */
# include <arpa/inet.h> /**< for inet_ntoa() */
# include <poll.h> /**< for poll() */
# include <csignal> /**< for signal() */
# include <string> /**< for string */
# include <algorithm> /**< for find() */
# include "Replies.hpp" /**< for Replies class */

//-------------------------------------------------------//
# define RED "\e[1;31m" /**< for red color */
# define WHI "\e[0;37m" /**< for white color */
# define GRE "\e[1;32m" /**< for green color */
# define YEL "\e[1;33m" /**< for yellow color */
//-------------------------------------------------------//

/**
 * @brief The Client class represents a client in the IRC server.
 */
class Client
{
	public:
		/**
		 * @brief Default constructor for the Client class.
		 */
		Client();

		/**
		 * @brief Getter for the client file descriptor.
		 * @return The client file descriptor.
		 */
		int get_fd() const;

		/**
		 * @brief Getter for the login status of the client.
		 * @return True if the client is logged in, false otherwise.
		 */
		bool get_is_logged() const;

		/**
		 * @brief Getter for the client buffer.
		 * @return The client buffer.
		 */
		std::string get_buffer() const;

		/**
		 * @brief Getter for the client nickname.
		 * @return The client nickname.
		 */
		std::string get_nickname() const;

		/**
		 * @brief Getter for the client username.
		 * @return The client username.
		 */
		std::string get_username() const;

		/**
		 * @brief Getter for the client password.
		 * @return The client password.
		 */
		std::string get_password() const;

		/**
 * @brief
		 * @return The client hostname.
		 */
		std::string get_hostname() const;

		/**
		 * @brief
		 * @return A vector of channel names.
		 */
		std::vector<std::string> get_channels_invited() const;

		/**
		 * @brief
		 * @return The client IP address.
		 */
		std::string get_ip_address() const;

		/**
		 * @brief
		 * @return True if the client is authenticated, false otherwise.
		 */
		bool get_is_authenticated() const;

		/**
		 * @brief
		 * @return True if the client is an operator, false otherwise.
		 */
		bool get_is_operator() const;

		/**
		 * @brief
		 * @param fd The client file descriptor.
		 */
		void set_fd(const int fd);

		/**
		 * @brief
		 * @param is_logged The login status of the client.
		 */
		void set_is_logged(bool is_logged);

		/**
		 * @brief
		 * @param ipadd The client IP address.
		 */
		void set_ip_add(const std::string &ipadd);

		/**
		 * @brief
		 * @param buffer The client buffer.
		 */
		void set_buffer(const std::string &buffer);

		/**
		 * @brief
		 * @param nickname The client nickname.
		 */
		void set_nickname(const std::string &nickname);

		/**
		 * @brief
		 * @param username The client username.
		 */
		void set_username(const std::string &username);

		/**
		 * @brief
		 * @param password The client password.
		 */
		void set_password(const std::string &password);

		/**
		 * @brief
		 * @param is_authenticated The authentication status of the client.
		 */
		void set_is_authenticated(bool is_authenticated);

		/**
		 * @brief
		 * @param is_operator The operator status of the client.
		 */
		void set_is_operator(bool is_operator);

		/**
		 * @brief
		 * @param channel The channel name.
		 * @return True if the client is invited to the channel, false otherwise.
		 */
		bool is_channel_invited(const std::string &channel);

		/**
		 * @brief
		 * @param channel The channel name.
		 */
		void add_channel_invited(const std::string &channel);

		/**
		 * @brief
		 * @param channel The channel name.
		 */
		void remove_channel_invited(const std::string &channel);

		/**
		 * @brief
		 * @param sender The client sending the message.
		 * @param target The target of the message.
		 * @param message The message to be broadcasted.
		 */
		void broadcast(Client *sender, std::string target, std::string message);

	private:
		int _fd; /**< The client file descriptor. */
		bool _is_logged; /**< The login status of the client. */
		std::string _buffer; /**< The client buffer. */
		bool _is_authenticated; /**< The authentication status of the client. */
		bool _is_operator; /**< The operator status of the client. */
		std::string _ip_addr; /**< The client IP address. */
		std::string _nickname; /**< The client nickname. */
		std::string _username; /**< The client username. */
		std::string _password; /**< The client password. */
		std::vector<std::string> _channels_invited; /**< The vector of channels invited. */
};

#endif // CLIENT_HPP
