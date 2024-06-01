/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:03:01 by yde-goes          #+#    #+#             */
/*   Updated: 2024/06/01 16:05:13 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#define EPSILON 0.0001

#include <sstream>
#include <string>
#include <vector>

std::pair<std::vector<std::string>, std::string> split_message(
	const std::string& message);

#endif	// UTILS_HPP/**
 