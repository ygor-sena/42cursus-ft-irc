/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 18:18:57 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/31 22:00:40 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

# define CRLF "\r\n"

/**
 * @brief Reply for successful connection to the IRC server.
 *
 * @param nickname The nickname of the user.
 * @return The reply message.
 */
# define RPL_CONNECTED(nickname) (": 001 " + nickname + " : Welcome to the IRC server!" + CRLF)

/**
 * @brief Reply for the user mode of a channel.
 *
 * @param hostname The hostname of the server.
 * @param channelname The name of the channel.
 * @param mode The mode of the channel.
 * @param user The user.
 * @return The reply message.
 */
# define RPL_UMODEIS(hostname, channelname, mode, user)  ":" + hostname + " MODE " + channelname + " " + mode + " " + user + CRLF

/**
 * @brief Reply for the creation time of a channel.
 *
 * @param nickname The nickname of the user.
 * @param channelname The name of the channel.
 * @param creationtime The creation time of the channel.
 * @return The reply message.
 */
# define RPL_CREATIONTIME(nickname, channelname, creationtime) ": 329 " + nickname + " " + channelname + " " + creationtime + CRLF

/**
 * @brief Reply for the channel modes.
 *
 * @param nickname The nickname of the user.
 * @param channelname The name of the channel.
 * @param modes The modes of the channel.
 * @return The reply message.
 */
# define RPL_CHANNELMODES(nickname, channelname, modes) ": 324 " + nickname + " " + channelname + " " + modes + CRLF

/**
 * @brief Reply for changing the mode of a channel.
 *
 * @param hostname The hostname of the server.
 * @param channelname The name of the channel.
 * @param mode The mode to be changed.
 * @param arguments The arguments for the mode change.
 * @return The reply message.
 */
# define RPL_CHANGEMODE(hostname, channelname, mode, arguments) (":" + hostname + " MODE " + channelname + " " + mode + " " + arguments + CRLF)

/**
 * @brief Reply for changing the nickname of a user.
 *
 * @param oldnickname The old nickname of the user.
 * @param nickname The new nickname of the user.
 * @return The reply message.
 */
# define RPL_NICKCHANGE(oldnickname, nickname) (":" + oldnickname + " NICK " + nickname + CRLF)

/**
 * @brief Reply for a user joining a channel.
 *
 * @param hostname The hostname of the server.
 * @param ipaddress The IP address of the user.
 * @param channelname The name of the channel.
 * @return The reply message.
 */
# define RPL_JOINMSG(hostname, ipaddress, channelname) (":" + hostname + "@" + ipaddress + " JOIN " + channelname + CRLF)

/**
 * @brief Reply for the list of clients in a channel.
 *
 * @param nickname The nickname of the user.
 * @param channelname The name of the channel.
 * @param clientslist The list of clients in the channel.
 * @return The reply message.
 */
# define RPL_NAMREPLY(nickname, channelname, clientslist) (": 353 " + nickname + " @ " + channelname + " :" + clientslist + CRLF)

/**
 * @brief Reply for the end of the list of clients in a channel.
 *
 * @param nickname The nickname of the user.
 * @param channelname The name of the channel.
 * @return The reply message.
 */
# define RPL_ENDOFNAMES(nickname, channelname) (": 366 " + nickname + " " + channelname + " :END of /NAMES list" + CRLF)

// Topic
/**
 * @brief Reply for the topic of a channel.
 *
 * @param nickname The nickname of the user.
 * @param channelname The name of the channel.
 * @param topic The topic of the channel.
 * @return The reply message.
 */
# define RPL_TOPIC(nickname, channelname, topic) (": 332 " + nickname + " " +channelname + " :" + topic + CRLF)

/**
 * @brief Reply for when no topic is set for a channel.
 *
 * @param nickname The nickname of the user.
 * @param channelname The name of the channel.
 * @return The reply message.
 */
# define RPL_NOTOPIC(nickname, channelname) (": 331 " + nickname + " " +channelname + " : No topic is set " + CRLF)

/**
 * @brief Reply for a user quitting the server.
 *
 * @param nickname The nickname of the user.
 * @return The reply message.
 */
# define RPL_QUITMESSAGE(nickname) (": 301 " + nickname + " :You have quit the server." + CRLF)

/**
 * @brief Reply for a user leaving a channel.
 *
 * @param hostname The hostname of the server.
 * @param channelname The name of the channel.
 * @param nickname The nickname of the user.
 * @return The reply message.
 */
# define RPL_PART(hostname, channelname, nickname) (":" + hostname + " PART " + channelname + " :" + nickname + " has left the channel" + CRLF)

/**
 * @brief Reply for a user being kicked from a channel.
 *
 * @param hostname The hostname of the server.
 * @param channelname The name of the channel.
 * @param kickername The nickname of the user who kicked.
 * @param kickedname The nickname of the user who was kicked.
 * @param comment The comment for the kick.
 * @return The reply message.
 */
# define RPL_KICK(hostname, channelname, kickername, kickedname, comment) (":" + hostname + " KICK " + channelname + " " + kickedname + " :" + kickername + " " + comment + CRLF)

/**
 * @brief Reply for inviting a user to a channel.
 *
 * @param hostname The hostname of the server.
 * @param channelname The name of the channel.
 * @param invitername The nickname of the user who invited.
 * @param invitedname The nickname of the user who was invited.
 * @return The reply message.
 */
# define RPL_INVITING(hostname, channelname, invitername, invitedname) (":" + hostname + " INVITE " + invitedname + " " + channelname + CRLF)

/**
 * @brief Reply for a private message.
 *
 * @param hostname The hostname of the server.
 * @param receiver The nickname of the receiver.
 * @param text The text of the message.
 * @return The reply message.
 */
# define RPL_PRIVMSG(hostname, receiver, text) (":" + hostname + " PRIVMSG " + receiver + " " + text + CRLF)

/**
 * @brief Reply for the marvin_bot command.
 *
 * @param nickname The nickname of the user.
 * @return The reply message.
 */
# define BOT_CMDMARVIN(nickname) (": 4242 marvin_bot " + nickname + " :Hello, I am Marvin, the paranoid robot." + CRLF)

/**
 * @brief Reply for the marvin_bot time command.
 *
 * @param nickname The nickname of the user.
 * @param time The server time.
 * @return The reply message.
 */
# define BOT_CMDTIME(nickname, time) (": 4242 marvin_bot " + nickname + " :Server time: " + time_str)

/**
 * @brief Reply for the marvin_bot whois command.
 *
 * @param nickname The nickname of the user.
 * @param username The username.
 * @param ipaddr The IP address.
 * @return The reply message.
 */
# define BOT_CMDWHOIS(nickname, username, ipaddr) (": 4242 marvin_bot " + nickname + " :Whois " + username + " " + ipaddr + " :End of WHOIS list" + CRLF)

/**
 * @brief Reply for the marvin_bot quote command.
 *
 * @param nickname The nickname of the user.
 * @param quote The quote.
 * @return The reply message.
 */
# define BOT_CMDQUOTE(nickname, quote) (": 4242 marvin_bot " + nickname + " :" + quote + CRLF)

/**
 * @brief Reply for when a client is not in a channel to use marvin commands.
 *
 * @param nickname The nickname of the user.
 * @return The reply message.
 */
# define BOT_CLIENTNOTINCHANNEL(nickname) (": 2424 marvin_bot " + nickname + ": Must be part of at least one channel to use marvin commands" + CRLF)

/**
 * @brief Reply for when a whois user does not exist.
 *
 * @param nickname The nickname of the user.
 * @return The reply message.
 */
# define BOT_WHOISDOESNTEXIST(nickname) (": 4242 marvin_bot " + nickname + " :No such user" + CRLF)

///////// ERRORS /////////

/**
 * @brief Error reply for the need of a parameter for a mode.
 *
 * @param channelname The name of the channel.
 * @param mode The mode.
 * @return The error message.
 */
# define ERR_NEEDMODEPARM(channelname, mode) (": 696 " + channelname + " * You must specify a parameter for the key mode. " + mode + CRLF)

/**
 * @brief Error reply for an invalid mode parameter.
 *
 * @param channelname The name of the channel.
 * @param mode The mode.
 * @return The error message.
 */
# define ERR_INVALIDMODEPARM(channelname, mode) (": 696 " + channelname + " Invalid mode parameter. " + mode + CRLF)

/**
 * @brief Error reply for a channel key already set.
 *
 * @param channelname The name of the channel.
 * @return The error message.
 */
# define ERR_KEYSET(channelname) ": 467 " + channelname + " Channel key already set. " + CRLF

/**
 * @brief Error reply for an unknown channel mode.
 *
 * @param nickname The nickname of the user.
 * @param channelname The name of the channel.
 * @param mode The mode.
 * @return The error message.
 */
# define ERR_UNKNOWNMODE(nickname, channelname, mode) ": 472 " + nickname + " " + channelname + " " + mode + " :is not a recognised channel mode" + CRLF

/**
 * @brief Error reply for not enough parameters.
 *
 * @param nickname The nickname of the user.
 * @return The error message.
 */
# define ERR_NEEDMOREPARAMS(nickname) (": 461 " + nickname + " :Not enough parameters." + CRLF)

/**
 * @brief Error reply for a channel not found.
 *
 * @param nickname The nickname of the user.
 * @param channelname The name of the channel.
 * @return The error message.
 */
# define ERR_CHANNELNOTFOUND(nickname, channelname) (": 403 " + nickname + " " + channelname + " :No such channel" + CRLF)

/**
 * @brief Error reply for not being a channel operator.
 *
 * @param channelname The name of the channel.
 * @return The error message.
 */
# define ERR_NOTOPERATOR(channelname) (": 482 " + channelname + " :You're not a channel operator" + CRLF)

/**
 * @brief Error reply for a nickname or channel not found.
 *
 * @param channelname The name of the channel.
 * @param name The nickname or channel name.
 * @return The error message.
 */
# define ERR_NOSUCHNICK(channelname, name) (": 401 " + channelname + " " + name + " :No such nick/channel" + CRLF)

/**
 * @brief Error reply for an incorrect password.
 *
 * @param nickname The nickname of the user.
 * @return The error message.
 */
# define ERR_INCORPASS(nickname) (": 464 " + nickname + " :Password incorrect !" + CRLF )

/**
 * @brief Error reply for no nickname given.
 *
 * @param nickname The nickname of the user.
 * @return The error message.
 */
# define ERR_NONICKNAME(nickname) (": 431 " + nickname + " :No nickname given" + CRLF )

/**
 * @brief Error reply for a nickname already in use.
 *
 * @param nickname The nickname of the user.
 * @return The error message.
 */
# define ERR_NICKINUSE(nickname) (": 433 " + nickname + " :Nickname is already in use" + CRLF)

/**
 * @brief Error reply for already being registered.
 *
 * @param nickname The nickname of the user.
 * @return The error message.
 */
# define ERR_ALREADYREGISTERED(nickname) (": 462 " + nickname + " :You may not reregister !" + CRLF )

/**
 * @brief Error reply for not being registered.
 *
 * @param nickname The nickname of the user.
 * @return The error message.
 */
# define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You have not registered!" + CRLF)

/**
 * @brief Error reply for an unknown command.
 *
 * @param nickname The nickname of the user.
 * @param command The unknown command.
 * @return The error message.
 */
# define ERR_CMDNOTFOUND(nickname, command) (": 421 " + nickname + " " + command + " :Unknown command" + CRLF)

/**
 * @brief Error reply for a bad channel key.
 *
 * @param nickname The nickname of the user.
 * @param channelname The name of the channel.
 * @return The error message.
 */
# define ERR_BADCHANNELKEY(nickname, channelname) (": 475 " + nickname + " " + channelname + " :Cannot join channel (incorrect key)" + CRLF)

/**
 * @brief Error reply for an invite-only channel.
 *
 * @param nickname The nickname of the user.
 * @param channelname The name of the channel.
 * @return The error message.
 */
# define ERR_INVITEONLYCHAN(nickname, channelname) (": 473 " + nickname + " " + channelname + " :Cannot join channel (+i)" + CRLF)

/**
 * @brief Error reply for a channel being full.
 *
 * @param nickname The nickname of the user.
 * @param channelname The name of the channel.
 * @return The error message.
 */
# define ERR_CHANNELISFULL(nickname, channelname) (": 422 " + nickname + " " + channelname + " :Cannot join channel (+l)" + CRLF)

/**
 * @brief Error reply for a non-existent channel.
 *
 * @param channelname The name of the channel.
 * @return The error message.
 */
# define ERR_NOSUCHCHANNEL(channelname) (": 403 " + channelname + " :No such channel" + CRLF)

/**
 * @brief Error reply for a user not being in a channel.
 *
 * @param nickname The nickname of the user.
 * @param channelname The name of the channel.
 * @return The error message.
 */
# define ERR_USERNOTINCHANNEL(nickname, channelname) ("441 " + nickname + " " + channelname + " :They aren't on that channel" + CRLF)

/**
 * @brief Error reply for not being on a channel.
 *
 * @param channelname The name of the channel.
 * @return The error message.
 */
# define ERR_NOTONCHANNEL(channelname) (": 442 " + channelname + " :You're not on that channel" + CRLF)

/**
 * @brief Error reply for a user already being on a channel.
 *
 * @param user The nickname of the user.
 * @param channelname The name of the channel.
 * @return The error message.
 */
# define ERR_USERONCHANNEL(user, channelname) (": 443 " + user + " " + channelname + " :is already on channel" + CRLF)

/**
 * @brief Error reply for channel operator privileges needed.
 *
 * @param channelname The name of the channel.
 * @return The error message.
 */
# define ERR_CHANOPRIVSNEEDED(channelname) (": 482 " + channelname + " :You're not a channel operator" + CRLF)

/**
 * @brief Error reply for no privileges.
 *
 * @param nickname The nickname of the user.
 * @return The error message.
 */
# define ERR_NOPRIVILEGES(nickname) (": 481 " + nickname + " :Permission Denied- You're not an IRC operator" + CRLF)

#endif // REPLIES_HPP
