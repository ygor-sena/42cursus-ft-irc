/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:24:04 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/27 09:28:11 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include "Replies.hpp"

class Channel
{
	public:
		Channel();
		~Channel();
		Channel(std::string name);

/*
** --------------------------------- GETTERS ----------------------------------
*/
		std::string get_name(void) const;
		std::string get_client_names(void) const;
		std::vector<Client *> get_operator_clients(void);
		std::vector<Client *> get_channel_clients(void);
		int get_clients_size(void) const;

		/*
		** ---------------------------------
		*SETTERS----------------------------------
		*/
		void set_channel_operator(Client *client);

		/*
		** ----------------- Member functions for CHANNEL MANAGEMENT
		*------------------
		*/
		void invite(Client *client);
		void join(Client *client);
		void kick(Client *client);
		void part(Client *client);
		void broadcast(Client *sender, std::string target, std::string message);

		/*
		** ------------------ Channel STATUS and checker functions
		*--------------------
		*/
		bool has_client(Client *client);
		bool has_key(void);
		bool is_channel_operator(std::string nickname);

	   private:
		int topic;
		std::string _name;
		std::string _key;  // -> stores channel key, if any
		std::string _created_at;

		bool _has_key;

		std::vector<Client *>
			_clients;  // -> list of clients that are channel members
		std::vector<Client *>
			_operator_clients;  // -> list of channel operators
};

#endif //CHANNEL_HPP