/**
 * @file Utils.cpp
 * @brief Implementation of utility functions for the project.
 */

#include "Utils.hpp"

#include <sstream>
#include <string>
#include <utility>
#include <vector>

/**
 * @brief Splits a message into receivers and text.
 *
 * This function takes a message as input and splits it into two parts: the receivers and the text.
 * The receivers are extracted from the message string, which is expected to be in the format "receiver1,receiver2,... receiverN text".
 * The receivers are separated by commas and the text follows after the last receiver.
 *
 * @param message The message to be split.
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
