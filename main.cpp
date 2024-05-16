/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:47:49 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/11 14:32:08 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int argc, char **argv)
{
	Server server;
	// if (argc != 3)
	// 	{std::cout << "Usage: " << argv[0] << " <port number> <password>" << std::endl; return 1;}
	std::cout << "---- SERVER ----" << std::endl;
	server.init("4444", "12345");
	std::cout << "The Server Closed!" << std::endl;
}