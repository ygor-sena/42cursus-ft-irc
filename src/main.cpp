/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/06/01 10:13:43 by caalbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */






#include "Server.hpp"

/* Use this line if you want to test the server with the automated tests
created by Criterion framework
server.init("4444", "12345"); */
int main(int argc, char** argv)
{
	Server server;

	if (argc != 3)
	{
		std::cout << "Usage: " << argv[0] << " <port number> <password>"
				  << std::endl;
		return 1;
	}
	std::cout << GRE << "---- WELCOME TO OUR IRC SERVER! ----" << WHI
			  << std::endl;
	server.init(argv[1], argv[2]);

	std::cout << RED << "The Server Closed!" << WHI << std::endl;
}
