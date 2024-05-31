/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:02:52 by yde-goes          #+#    #+#             */
/*   Updated: 2024/05/24 18:27:47 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

#include <sstream>
#include <string>
#include <utility>
#include <vector>

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
