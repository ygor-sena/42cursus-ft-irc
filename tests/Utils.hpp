/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:03:01 by yde-goes          #+#    #+#             */
/*   Updated: 2024/05/24 18:51:12 by yde-goes         ###   ########.fr       */
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

#endif	// UTILS_HPP