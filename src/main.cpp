/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:47:49 by gilmar            #+#    #+#             */
/*   Updated: 2024/06/01 09:54:15 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/**
 * @brief The entry point of the program.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments.
 * @return int The exit status of the program.
 */
int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	Server server;
	if (argc != 3)
		{std::cout << YEL << "Usage: " << argv[0] << " <port number> <password>"<< WHI <<
	std::endl; return 1;}
	std::cout << GRE << "---- SERVER ----" << WHI << std::endl;
	// server.init("4444", "12345");
	std::cout << RED << "The Server Closed!" << WHI << std::endl;
}
