/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:02:52 by yde-goes          #+#    #+#             */
/*   Updated: 2024/06/01 16:04:32 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

#include <sstream>
#include <string>
/**
 * @brief Splits a message into receivers and text.
 *
 * This function takes a message string and splits it into individual receivers
 * and the text. The receivers are separated by commas and the text follows
 * after a space.
 *
 * @param message The message string to be split.
 * @return A pair containing a vector of receivers and the text.
 */
std::pair<std::vector<std::string>, std::string> split_message(
	const std::string& message)
{
	std::istringstream iss(message);
	std::string receiversStr, text;

	// Get the receivers string and the text
	std::getline(iss, receiversStr, ' ');
	std::getline(iss, text);

	// Split the receivers string into individual receivers
	std::vector<std::string> receivers;
	std::istringstream receiversStream(receiversStr);
	std::string receiver;
	while (std::getline(receiversStream, receiver, ','))
	{
		receivers.push_back(receiver);
	}

	// Create a pair and return it
	std::pair<std::vector<std::string>, std::string> result;
	result.first = receivers;
	result.second = text;
	return result;
}
