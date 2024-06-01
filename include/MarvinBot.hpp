/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MarvinBot.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:31:32 by caalbert          #+#    #+#             */
/*   Updated: 2024/06/01 11:56:27 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MARVINBOT_HPP
#define MARVINBOT_HPP

#include "Replies.hpp"
#include "Server.hpp"

#define SOCRATES_KNOWLEDGE "I know that I know nothing. - Socrates"
#define DESCARTES_EXISTENCE "I think, therefore I am. - René Descartes"
#define KANT_CATEGORICAL_IMPERATIVE                                        \
	"Act only according to that maxim whereby you can, at the same time, " \
	"will that it should become a universal law. - Immanuel Kant"
#define NIETZSCHE_GOD "God is dead! - Friedrich Nietzsche"
#define PLATO_FORMS "The Forms are eternal and changeless. - Plato"
#define MARX_RELIGION "Religion is the opium of the people. - Karl Marx"
#define CONFUCIUS_WISDOM \
	"Real knowledge is to know the extent of one's ignorance. - Confucius"
#define HUME_SCIENCE \
	"A wise man proportions his belief to the evidence. - David Hume"
#define HEIDEGGER_BEING "Being is time, and time is finite. - Martin Heidegger"
#define ROUSSEAU_FREEDOM                                                \
	"Man is born free, and everywhere he is in chains. - Jean-Jacques " \
	"Rousseau"

/**
 * @class MarvinBot
 * @brief Represents a MarvinBot object.
 *
 * The MarvinBot class is responsible for managing quotes and returning a random quote from a predefined list.
 */
class MarvinBot
{
	public:
		/**
		 * @brief Default constructor for MarvinBot.
		 */
		MarvinBot();

		/**
		 * @brief Destructor for MarvinBot.
		 */
		~MarvinBot();

		/**
		 * @brief Enum for quotes.
		 */
		enum EnumMarvinBotQuotes
		{
				QUOTE_SOCRATES_KNOWLEDGE,                /**< Quote: "I know that I know nothing. - Socrates" */
				QUOTE_DESCARTES_EXISTENCE,               /**< Quote: "I think, therefore I am. - René Descartes" */
				QUOTE_KANT_CATEGORICAL_IMPERATIVE,        /**< Quote: "Act only according to that maxim whereby you can, at the same time, will that it should become a universal law. - Immanuel Kant" */
				QUOTE_NIETZSCHE_GOD,                     /**< Quote: "God is dead! - Friedrich Nietzsche" */
				QUOTE_PLATO_FORMS,                       /**< Quote: "The Forms are eternal and changeless. - Plato" */
				QUOTE_MARX_RELIGION,                      /**< Quote: "Religion is the opium of the people. - Karl Marx" */
				QUOTE_CONFUCIUS_WISDOM,                   /**< Quote: "Real knowledge is to know the extent of one's ignorance. - Confucius" */
				QUOTE_HUME_SCIENCE,                       /**< Quote: "A wise man proportions his belief to the evidence. - David Hume" */
				QUOTE_HEIDEGGER_BEING,                    /**< Quote: "Being is time, and time is finite. - Martin Heidegger" */
				QUOTE_ROUSSEAU_FREEDOM,                   /**< Quote: "Man is born free, and everywhere he is in chains. - Jean-Jacques Rousseau" */
				QUOTES_COUNT                             /**< To keep track of the number of quotes */
		};

		const char* marvin_bot_quotes[QUOTES_COUNT]; /**< Array of quotes */

	private:
		/**
 		 * @brief Generates and returns a random quote from the list of quotes.
		 * @return A random quote.
		 */
		std::string _return_quote();
};

#endif  // MARVINBOT_HPP
