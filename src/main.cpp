/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:47:49 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/24 18:27:14 by yde-goes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	Server server;
	// if (argc != 3)
	// 	{std::cout << "Usage: " << argv[0] << " <port number> <password>" << std::endl; return 1;}
	std::cout << "---- SERVER ----" << std::endl;
	server.init("4444", "12345");
	std::cout << "The Server Closed!" << std::endl;
}