/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MarvinBot.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:31:32 by caalbert          #+#    #+#             */
/*   Updated: 2024/05/31 21:58:03 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MARVINBOT_HPP
# define MARVINBOT_HPP

# include "Server.hpp" /**< for Server class */
# include "Replies.hpp" /**< for Replies class */

/**
 * @brief The MarvinBot class represents a bot named Marvin.
 */
class MarvinBot
{
	public:
		/**
		 * @brief Default constructor for MarvinBot.
		 */
		MarvinBot();

		/**
		 * @brief Default destructor for MarvinBot.
		 */
		~MarvinBot();

		// Enum for quotes
		enum EnumMarvinBotQuotes {
			QUOTE_SOCRATES_KNOWLEDGE, /**< Quote by Socrates */
			QUOTE_DESCARTES_EXISTENCE, /**< Quote by René Descartes */
			QUOTE_KANT_CATEGORICAL_IMPERATIVE, /**< Quote by Immanuel Kant */
			QUOTE_NIETZSCHE_GOD, /**< Quote by Friedrich Nietzsche */
			QUOTE_PLATO_FORMS, /**< Quote by Plato */
			QUOTE_MARX_RELIGION, /**< Quote by Karl Marx */
			QUOTE_CONFUCIUS_WISDOM, /**< Quote by Confucius */
			QUOTE_HUME_SCIENCE, /**< Quote by David Hume */
			QUOTE_HEIDEGGER_BEING, /**< Quote by Martin Heidegger */
			QUOTE_ROUSSEAU_FREEDOM, /**< Quote by Jean-Jacques Rousseau */
			QUOTES_COUNT /**< Number of quotes */
		};

		const char* marvin_bot_quotes[QUOTES_COUNT]; /**< Array of MarvinBot quotes */

	private:
		/**
		 * @brief Returns a random quote from the marvin_bot_quotes array.
		 * @return A random quote.
		 */
		std::string _return_quote(); /**< Returns a random quote from the marvin_bot_quotes array */
};

#endif // MARVINBOT_HPP