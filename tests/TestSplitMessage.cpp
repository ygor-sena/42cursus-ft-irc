/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestSplitMessage.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:03:20 by yde-goes          #+#    #+#             */
/*   Updated: 2024/06/01 16:01:57 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <string>
#include <utility>

/**
 * @brief Test case for splitting a message into receivers and text.
 *
 * This test case checks if the split_message function correctly splits a message
 * into receivers and text.
 */
Test(split_message, should_split_message_into_receivers_and_text_1)
{
	std::string message = "receiver1,receiver2,receiver3 This is a test message";

	std::pair<std::vector<std::string>, std::string> result = split_message(message);

	std::vector<std::string> expectedReceivers = {"receiver1", "receiver2", "receiver3"};
	std::string expectedText = "This is a test message";

	cr_assert_eq(result.first.size(), expectedReceivers.size(), "Number of receivers does not match");
	for (size_t i = 0; i < expectedReceivers.size(); i++)
	{
		cr_assert_str_eq(result.first[i].c_str(), expectedReceivers[i].c_str(), "Receiver %zu does not match", i);
	}

	cr_assert_str_eq(result.second.c_str(), expectedText.c_str(), "Text does not match");
}

/**
 * @brief Test case for splitting a message into receivers and text.
 *
 * This test case checks if the split_message function correctly splits a message
 * into receivers and text.
 */
Test(split_message, should_split_message_into_receivers_and_text_2)
{
	std::string message = "receiver1,receiver2,receiver3, This is a test message";

	std::pair<std::vector<std::string>, std::string> result = split_message(message);

	std::vector<std::string> expectedReceivers = {"receiver1", "receiver2", "receiver3"};
	std::string expectedText = "This is a test message";

	cr_assert_eq(result.first.size(), expectedReceivers.size(), "Number of receivers does not match");
	for (size_t i = 0; i < expectedReceivers.size(); i++)
	{
		cr_assert_str_eq(result.first[i].c_str(), expectedReceivers[i].c_str(), "Receiver %zu does not match", i);
	}

	cr_assert_str_eq(result.second.c_str(), expectedText.c_str(), "Text does not match");
}

/**
 * @brief Test case for splitting a message into receivers and text.
 *
 * This test case checks if the split_message function correctly splits a message
 * into receivers and text.
 */
Test(split_message, should_split_message_into_receivers_and_text_3)
{
	std::string message = "channel This is a test message";

	std::pair<std::vector<std::string>, std::string> result = split_message(message);

	std::vector<std::string> expectedReceivers = {"channel"};
	std::string expectedText = "This is a test message";

	cr_assert_eq(result.first.size(), expectedReceivers.size(), "Number of receivers does not match");
	for (size_t i = 0; i < expectedReceivers.size(); i++)
	{
		cr_assert_str_eq(result.first[i].c_str(), expectedReceivers[i].c_str(), "Receiver %zu does not match", i);
	}

	cr_assert_str_eq(result.second.c_str(), expectedText.c_str(), "Text does not match");
}
