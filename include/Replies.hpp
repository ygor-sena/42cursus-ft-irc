/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 18:18:57 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/29 16:22:57 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

# define CRLF "\r\n"

// refer to https://datatracker.ietf.org/doc/html/rfc1459

# define RPL_CONNECTED(nickname) (": 001 " + nickname + " : Welcome to the IRC server!" + CRLF)

# define RPL_UMODEIS(hostname, channelname, mode, user)  ":" + hostname + " MODE " + channelname + " " + mode + " " + user + CRLF

# define RPL_CREATIONTIME(nickname, channelname, creationtime) ": 329 " + nickname + " " + channelname + " " + creationtime + CRLF

# define RPL_CHANNELMODES(nickname, channelname, modes) ": 324 " + nickname + " " + channelname + " " + modes + CRLF

# define RPL_CHANGEMODE(hostname, channelname, mode, arguments) (":" + hostname + " MODE " + channelname + " " + mode + " " + arguments + CRLF)

# define RPL_NICKCHANGE(oldnickname, nickname) (":" + oldnickname + " NICK " + nickname + CRLF)

# define RPL_JOINMSG(hostname, ipaddress, channelname) (":" + hostname + "@" + ipaddress + " JOIN " + channelname + CRLF)

# define RPL_NAMREPLY(nickname, channelname, clientslist) (": 353 " + nickname + " @ " + channelname + " :" + clientslist + CRLF)

# define RPL_ENDOFNAMES(nickname, channelname) (": 366 " + nickname + " " + channelname + " :END of /NAMES list" + CRLF)

// Topic
# define RPL_TOPIC(nickname, channelname, topic) (": 332 " + nickname + " " +channelname + " :" + topic + CRLF)
# define RPL_NOTOPIC(nickname, channelname) (": 331 " + nickname + " " +channelname + " : No topic is set " + CRLF)

# define RPL_QUITMESSAGE(nickname) (": 301 " + nickname + " :You have quit the server." + CRLF)
# define RPL_PART(hostname, channelname, nickname) (":" + hostname + " PART " + channelname + " :" + nickname + " has left the channel" + CRLF)
# define RPL_KICK(hostname, channelname, kickername, kickedname, comment) (":" + hostname + " KICK " + channelname + " " + kickedname + " :" + kickername + " " + comment + CRLF)
# define RPL_INVITING(hostname, channelname, invitername, invitedname) (":" + hostname + " INVITE " + invitedname + " " + channelname + CRLF)
# define RPL_PRIVMSG(hostname, receiver, text) (":" + hostname + " PRIVMSG " + receiver + " " + text + CRLF)

///////// ERRORS /////////
# define ERR_NEEDMODEPARM(channelname, mode) (": 696 " + channelname + " * You must specify a parameter for the key mode. " + mode + CRLF)
# define ERR_INVALIDMODEPARM(channelname, mode) (": 696 " + channelname + " Invalid mode parameter. " + mode + CRLF)
# define ERR_KEYSET(channelname) ": 467 " + channelname + " Channel key already set. " + CRLF
# define ERR_UNKNOWNMODE(nickname, channelname, mode) ": 472 " + nickname + " " + channelname + " " + mode + " :is not a recognised channel mode" + CRLF
# define ERR_NEEDMOREPARAMS(nickname) (": 461 " + nickname + " :Not enough parameters." + CRLF)
# define ERR_CHANNELNOTFOUND(nickname, channelname) (": 403 " + nickname + " " + channelname + " :No such channel" + CRLF)
# define ERR_NOTOPERATOR(channelname) (": 482 " + channelname + " :You're not a channel operator" + CRLF)
# define ERR_NOSUCHNICK(channelname, name) (": 401 " + channelname + " " + name + " :No such nick/channel" + CRLF)
# define ERR_INCORPASS(nickname) (": 464 " + nickname + " :Password incorrect !" + CRLF )
# define ERR_NONICKNAME(nickname) (": 431 " + nickname + " :No nickname given" + CRLF )
# define ERR_NICKINUSE(nickname) (": 433 " + nickname + " :Nickname is already in use" + CRLF)


# define ERR_ALREADYREGISTERED(nickname) (": 462 " + nickname + " :You may not reregister !" + CRLF )
# define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You have not registered!" + CRLF)

# define ERR_CMDNOTFOUND(nickname, command) (": 421 " + nickname + " " + command + " :Unknown command" + CRLF)

# define ERR_BADCHANNELKEY(clientnickname, channelname) (": 475 " + clientnickname + " " + channelname + " :Cannot join channel (incorrect key)" + CRLF)
# define ERR_INVITEONLYCHAN(clientnickname, channelname) (": 473 " + clientnickname + " " + channelname + " :Cannot join channel (+i)" + CRLF)
# define ERR_CHANNELISFULL(clientnickname, channelname) (": 422 " + clientnickname + " " + channelname + " :Cannot join channel (+l)" + CRLF)
# define ERR_NOSUCHCHANNEL(channelname) (": 403 " + channelname + " :No such channel" + CRLF)
# define ERR_USERNOTINCHANNEL(nickname, channelname) ("441 " + nickname + " " + channelname + " :They aren't on that channel" + CRLF)
# define ERR_NOTONCHANNEL(channelname) (": 442 " + channelname + " :You're not on that channel" + CRLF)
# define ERR_USERONCHANNEL(user, channelname) (": 443 " + user + " " + channelname + " :is already on channel" + CRLF)
# define ERR_CHANOPRIVSNEEDED(channelname) (": 482 " + channelname + " :You're not a channel operator" + CRLF)
# define ERR_NOPRIVILEGES(nickname) (": 481 " + nickname + " :Permission Denied- You're not an IRC operator" + CRLF)

#endif // REPLIES_HPP
