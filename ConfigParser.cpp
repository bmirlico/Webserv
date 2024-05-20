/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmirlico <bmirlico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:25:17 by bmirlico          #+#    #+#             */
/*   Updated: 2024/05/20 03:54:34 by bmirlico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser(void) {}

ConfigParser::~ConfigParser(void) {}

ConfigParser::ConfigParser(const ConfigParser &cpy)
{
	*this = cpy;
}

ConfigParser& ConfigParser::operator=(const ConfigParser &rhs)
{
	// if (this != &rhs)
	// 	this->_servers = rhs.getServers();
	(void)rhs;
	return (*this);
}

// fonction qui permet de créer un vector de block server (qui sont des instances de la classe ServerConfig)
void ConfigParser::createServers(const std::string &configFile)
{
	std::string contentFile;
	ConfigFile	file(configFile);

	if (file.getTypeFilePath(file.getFilePath()) == 2)
		throw ErrorException("The config file you entered is a directory.");
	if (file.fileExistsandReadable(file.getFilePath()) == -1)
		throw ErrorException("The config file you entered does not exist.");
	if (file.fileExistsandReadable(file.getFilePath()) == 0)
		throw ErrorException("The config file you entered exists but is not readable.");
	contentFile = file.readConfigFile(configFile);
	if (contentFile.empty())
		throw ErrorException("The config file is empty.");
	removeComments(contentFile);
	removeSpaces(contentFile);
	//std::cout << contentFile << std::endl;
	createServerBlocks(contentFile);
	// check si le nb de servers est égal à la size du vector de serverConfigs ?
	for (size_t i = 0; i < this->_serverConfigs.size(); i++)
	{
		ServerConfig server;
		createServer(this->_serverConfigs[i], server);
		this->_servers.push_back(server);
	}
	// if (this->_nbServers > 1)
	// 	checkServers();
}

// enlève les commentaires en fin de chaque ligne, si jamais on nous envoie un fichier de conf avec des comments
void ConfigParser::removeComments(std::string &content)
{
	size_t pos;

	pos = content.find('#');
	while (pos != std::string::npos)
	{
		size_t pos_end;
		pos_end = content.find('\n', pos);
		content.erase(pos, pos_end - pos);
		pos = content.find('#');
	}
}

// enlève les espaces en début et en fin de fichier, pour trim le fichier de conf
void ConfigParser::removeSpaces(std::string &content)
{
	size_t	i = 0;

	while (content[i] && isspace(content[i]))
		i++;
	content = content.substr(i);
	i = content.length() - 1;
	while (i > 0 && isspace(content[i]))
		i--;
	content = content.substr(0, i + 1);
}

void ConfigParser::createServerBlocks(std::string &content)
{
	size_t start = 0;
	size_t end = 1;

	if (content.find("server", 0) == std::string::npos)
		throw ErrorException("Block Server not found in config file.");
	while (start != end && start < content.length())
	{
		start = findStartServerBlock(start, content);
		end = findEndServerBlock(start, content);
		if (start == end)
			throw ErrorException("Block Server scope is missing.");
		this->_serverConfigs.push_back(content.substr(start, end - start + 1));
		//this->_nb_server++;
		start = end + 1;
	}
}

size_t ConfigParser::findStartServerBlock(size_t start, std::string &content)
{
	size_t i;

	for (i = start; content[i]; i++)
	{
		if (content[i] == 's')
			break ;
		if (!isspace(content[i]))
			throw  ErrorException("1/Server block not properly defined with a wrong character => Server {}");
	}
	if (!content[i])
		return (start);
	if (content.compare(i, 6, "server") != 0)
		throw ErrorException("Server block does not start with: Server {}");
	i += 6;
	while (content[i] && isspace(content[i]))
		i++;
	if (content[i] == '{')
		return (i);
	else
		throw  ErrorException("2/Server block not properly defined with a wrong character => Server {}");
}

size_t ConfigParser::findEndServerBlock(size_t start, std::string &content)
{
	size_t	i;
	size_t	scope;
	
	scope = 0;
	for (i = start + 1; content[i]; i++)
	{
		if (content[i] == '{')
			scope++;
		if (content[i] == '}')
		{
			if (!scope)
				return (i);
			scope--;
		}
	}
	return (start);
}

void ConfigParser::createServer(std::string &config, ServerConfig &server)
{
	//TBD
}

std::vector<ServerConfig> ConfigParser::getServers() const
{
	return (this->_servers);
}


