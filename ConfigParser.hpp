/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmirlico <bmirlico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:08:32 by bmirlico          #+#    #+#             */
/*   Updated: 2024/05/19 23:26:52 by bmirlico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "ServerConfig.hpp"

class ServerConfig; 

class ConfigParser
{
	private:
		std::vector<ServerConfig> _servers;
		std::vector<std::string>	_serverConfigs;
	public:
		ConfigParser(void);
		~ConfigParser(void);
		ConfigParser(const ConfigParser &cpy);
		ConfigParser& operator=(const ConfigParser &rhs);
		bool parse(const std::string &configFile);
		void createServers(const std::string &configFile);
		void removeComments(std::string &content);
		void removeSpaces(std::string &content);
		void createServerBlocks(std::string &content);
		size_t findStartServerBlock(size_t start, std::string &content);
		size_t findEndServerBlock(size_t start, std::string &content);
		void createServer(std::string &config, ServerConfig &server);
		std::vector<ServerConfig> getServers() const;
		//
		public:
		class ErrorException: public std::exception
		{
			private:
				std::string _message;
			public:
				ErrorException(std::string message) throw()
				{
					_message = "CONFIG PARSER ERROR: " + message;
				}
				virtual const char* what() const throw()
				{
					return (_message.c_str());
				}
				virtual ~ErrorException() throw () {}
		};
};

#endif