/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:47:49 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/26 11:25:34 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	Server server;
	// if (argc != 3)
	// 	{std::cout << "Usage: " << argv[0] << " <port number> <password>" <<
	// std::endl; return 1;}
	std::cout << GRE << "---- SERVER ----" << WHI << std::endl;
	server.init("4444", "12345");
	std::cout << RED << "The Server Closed!" << WHI << std::endl;
}
