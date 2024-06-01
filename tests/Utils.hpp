/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:03:01 by yde-goes          #+#    #+#             */
/*   Updated: 2024/06/01 11:29:06 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#define EPSILON 0.0001

#include <sstream>
#include <string>
#include <vector>

/**
 * @brief Splits a message into a vector of strings and a string.
 *
 * @param message The message to be split.
 * @return std::pair<std::vector<std::string>, std::string> The split result.
 */
std::pair<std::vector<std::string>, std::string> split_message(
	const std::string& message);

#endif  // UTILS_HPP