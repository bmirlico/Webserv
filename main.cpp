/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmirlico <bmirlico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:07:18 by bmirlico          #+#    #+#             */
/*   Updated: 2024/05/20 03:10:31 by bmirlico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

// Explications du PARSING
// Attention, bien check les chemins relatifs et absolus de chaque fichier dans la config

int main(int ac, char **av)
{
	if (ac == 2) // ou ac == 1
	{
		try
		{
			ConfigParser servers;
			std::string configFile(av[1]);
			servers.createServers(configFile);
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
