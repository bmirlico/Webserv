/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmirlico <bmirlico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:07:18 by bmirlico          #+#    #+#             */
/*   Updated: 2024/05/23 02:44:08 by bmirlico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

// Explications du PARSING
// Attention, bien check les chemins relatifs et absolus de chaque fichier dans la config

int main(int ac, char **av)
{
	if (ac == 1 || ac == 2)
	{
		try
		{
			ConfigParser servers;
			std::string configFile;
			configFile = (ac == 1 ? "test.conf" : av[1]);
			servers.createServers(configFile);
			servers.printServers();
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			return (1);
		}
	}
	else
	{
		std::cout << "Error. The server is required one config file only as input." << std::endl;
		return (1);
	}
}
