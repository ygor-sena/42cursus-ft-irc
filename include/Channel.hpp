/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:24:04 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/31 21:40:46 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "Replies.hpp"

/**
* @class Channel
* @brief Represents a channel in an IRC server.
 *
* The Channel class provides functionality to manage a channel, including
* adding and removing clients, setting channel properties, and performing
* channel operations such as joining, kicking, and broadcasting messages.
 */
class Channel
{
  public:
    /**
     * @brief Default constructor.
     */
    Channel();

    /**
     * @brief Destructor.
     */
    ~Channel();

    /**
     * @brief Constructor with channel name.
     * @param name The name of the channel.
     */
    Channel(std::string name);

    /**
     * @brief Get the number of clients in the channel.
     * @return The number of clients in the channel.
     */
    int get_clients_size(void) const;

    /**
     * @brief Get the name of the channel.
     * @return The name of the channel.
     */
    std::string get_name(void) const;

    /**
     * @brief Get the topic of the channel.
     * @return The topic of the channel.
     */
    std::string get_topic(void) const;

    /**
     * @brief Get the channel key.
     * @return The channel key.
     */
    std::string get_channel_key(void) const;

    /**
     * @brief Get the names of the clients in the channel.
     * @return The names of the clients in the channel.
     */
    std::string get_client_names(void) const;

    /**
     * @brief Get the vector of channel clients.
     * @return The vector of channel clients.
     */
    std::vector<Client*> get_channel_clients(void);

    /**
     * @brief Get the vector of operator clients.
     * @return The vector of operator clients.
     */
    std::vector<Client*> get_operator_clients(void);

    /**
     * @brief Check if the channel has topic restriction.
     * @return True if the channel has topic restriction, false otherwise.
     */
    bool get_topic_restriction(void) const;

    /**
     * @brief Set the limit of clients in the channel.
     * @param limit The limit of clients in the channel.
     */
    void set_limit(int limit);

    /**
     * @brief Set the channel as invite-only.
     */
    void set_invite_only(void);

    /**
     * @brief Set the channel with topic restriction.
     */
    void set_topic_restriction(void);

    /**
     * @brief Set the topic of the channel.
     * @param topic The topic of the channel.
     */
    void set_topic(std::string topic);

    /**
     * @brief Set the key of the channel.
     * @param password The key of the channel.
     */
    void set_key(std::string password);

    /**
     * @brief Set a client as a channel operator.
     * @param client The client to set as a channel operator.
     */
    void set_channel_operator(Client* client);

    /**
     * @brief Remove the key of the channel.
     */
    void remove_key(void);

    /**
     * @brief Remove the limit of clients in the channel.
     */
    void remove_limit(void);

    /**
     * @brief Remove the invite-only restriction of the channel.
     */
    void remove_invite_only(void);

    /**
     * @brief Remove the topic restriction of the channel.
     */
    void remove_topic_restriction(void);

    /**
     * @brief Remove a client as a channel operator.
     * @param client The client to remove as a channel operator.
     */
    void remove_channel_operator(Client* client);

    /**
     * @brief Remove a client from the channel.
     * @param client The client to remove from the channel.
     */
    void remove_channel_client(Client* client);

    /**
     * @brief Join a client to the channel.
     * @param client The client to join to the channel.
     */
    void join(Client* client);

    /**
     * @brief Kick a client from the channel.
     * @param client The client to kick from the channel.
     */
    void kick(Client* client);

    /**
     * @brief Remove a client from the channel.
     * @param client The client to remove from the channel.
     */
    void part(Client* client);

    /**
     * @brief Quit a client from the channel.
     * @param client The client to quit from the channel.
     */
    void quit(Client* client);

    /**
     * @brief Invite a client to the channel.
     * @param client The client to invite to the channel.
     */
    void invite(Client* client);

    /**
     * @brief Broadcast a message to the channel.
     * @param sender The client sending the message.
     * @param target The target of the message.
     * @param message The message to broadcast.
     */
    void broadcast(Client* sender, std::string target, std::string message);

    /**
     * @brief Check if the channel has a key.
     * @return True if the channel has a key, false otherwise.
     */
    bool has_key(void) const;

    /**
     * @brief Check if a client is in the channel.
     * @param client The client to check.
     * @return True if the client is in the channel, false otherwise.
     */
    bool has_client(Client* client);

    /**
     * @brief Check if the channel is full.
     * @return True if the channel is full, false otherwise.
     */
    bool is_channel_full(void) const;

    /**
     * @brief Check if a client is a channel operator.
     * @param fd The file descriptor of the client.
     * @return True if the client is a channel operator, false otherwise.
     */
    bool is_channel_operator(const int fd);

    /**
     * @brief Check if the channel is invite-only.
     * @return True if the channel is invite-only, false otherwise.
     */
    bool is_channel_invite_only(void) const;

    /**
     * @brief Check if a client is a channel operator.
     * @param nickname The nickname of the client.
     * @return True if the client is a channel operator, false otherwise.
     */
    bool is_channel_operator(std::string nickname);

    /**
     * @brief Check if a client is in the channel.
     * @param nickname The nickname of the client.
     * @return True if the client is in the channel, false otherwise.
     */
    bool is_client_in_channel(std::string nickname);

  private:
    int _limit; /**< The limit of clients in the channel. */
    bool _has_key; /**< Flag indicating if the channel has a key. */
    bool _invite_only; /**< Flag indicating if the channel is invite-only. */
    bool _topic_restriction; /**< Flag indicating if the channel has topic restriction. */
    std::string _key; /**< The key of the channel. */
    std::string _name; /**< The name of the channel. */
    std::string _topic; /**< The topic of the channel. */
    std::string _created_at; /**< The creation timestamp of the channel. */
    std::vector<Client*> _clients; /**< The list of clients that are channel members. */
    std::vector<Client*> _operator_clients; /**< The list of channel operators. */
};

#endif  // CHANNEL_HPP
