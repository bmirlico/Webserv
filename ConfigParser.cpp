/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmirlico <bmirlico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:25:17 by bmirlico          #+#    #+#             */
/*   Updated: 2024/05/24 12:57:30 by bmirlico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser(void) : _nbServers(0) {}

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


void ConfigParser::printServers(void)
{
	std::cout << "------------- Config -------------" << std::endl;
	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << "Server #" << i + 1 << std::endl;
		std::cout << "Server name: " << _servers[i].getServerName() << std::endl;
		std::cout << "Host: " << _servers[i].getHost() << std::endl;
		std::cout << "Root: " << _servers[i].getRoot() << std::endl;
		std::cout << "Index: " << _servers[i].getIndex() << std::endl;
		std::cout << "Port: " << _servers[i].getPort() << std::endl;
		std::cout << "Max Body Size: " << _servers[i].getClientMaxBodySize() << std::endl;
		std::cout << "Error pages: " << _servers[i].getErrorPages().size() << std::endl;
		std::map<int, std::string>::const_iterator it = _servers[i].getErrorPages().begin();
		while (it != _servers[i].getErrorPages().end())
		{
			std::cout << it->first << " - " << it->second << std::endl;
			++it;
		}
		std::cout << "Locations: " << _servers[i].getLocations().size() << std::endl;
		std::vector<Location>::const_iterator itl = _servers[i].getLocations().begin();
		while (itl != _servers[i].getLocations().end())
		{
			std::cout << "name location: " << itl->getPath() << std::endl;
			std::cout << "methods: " << itl->getPrintMethods() << std::endl;
			std::cout << "index: " << itl->getIndexLocation() << std::endl;
			std::cout << "Max Body Size loc: " << itl->getMaxBodySizeLoc() << std::endl;
			if (itl->getCgiExtension().size() == 0)
			{
				std::cout << "root: " << itl->getRootLocation() << std::endl;
				if (!itl->getReturn().empty())
					std::cout << "return: " << itl->getReturn() << std::endl;
				if (!itl->getAlias().empty())
					std::cout << "alias: " << itl->getAlias() << std::endl;
			}
			else
			{
				std::cout << "cgi root: " << itl->getRootLocation() << std::endl;
				std::cout << "cgi_interpreters: " << std::endl;
				for (size_t i = 0; i < itl->getCgiInterpreter().size(); i++)
					std::cout << itl->getCgiInterpreter()[i] << std::endl;
				std::cout << "cgi_ext: " << std::endl;
				for (size_t i = 0; i < itl->getCgiExtension().size(); i++)
					std::cout << itl->getCgiExtension()[i] << std::endl;
			}
			++itl;
		}
		itl = _servers[i].getLocations().begin();
		std::cout << "-----------------------------" << std::endl;
	}
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
	createServerBlocks(contentFile);
	// check si le nb de servers est égal à la size du vector de serverConfigs ?
	for (size_t i = 0; i < this->_serverConfigs.size(); i++)
	{
		ServerConfig server;
		createServer(this->_serverConfigs[i], server);
		this->_servers.push_back(server);
	}
	if (this->_nbServers > 1)
		checkDupServers();
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
		this->_nbServers++;
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

std::vector<std::string> ConfigParser::splitServerBlock(std::string line, std::string sep)
{
	std::vector<std::string>	str;
	std::string::size_type		start, end;

	start = 0;
	end = 0;
	while (1)
	{
		end = line.find_first_of(sep, start);
		if (end == std::string::npos)
			break;
		std::string tmp = line.substr(start, end - start);
		str.push_back(tmp);
		start = line.find_first_not_of(sep, end);
		if (start == std::string::npos)
			break;
	}
	return (str);
}

void ConfigParser::createServer(std::string &config, ServerConfig &server)
{
	std::vector<std::string>	input;
	std::vector<std::string>	error_codes;
	int		flagLoc = 1;
	bool	flagAutoIndex = false;
	bool	flagMaxSize = false;

	input = splitServerBlock(config += ' ', std::string(" \n\t"));
	if (input.size() < 3) // pourquoi est ce pertinent ici ? je peux le faire après
		throw  ErrorException("Failed server validation.");
	for (size_t i = 0; i < input.size(); i++)
	{
		if (input[i] == "listen" && (i + 1) < input.size() && flagLoc)
		{
			if (server.getPort())
				throw  ErrorException("Port is duplicated");
			server.setPort(input[++i]);
		}
		else if (input[i] == "host" && (i + 1) < input.size() && flagLoc)
		{
			if (!server.getHost().empty())
				throw  ErrorException("Host is duplicated");
			server.setHost(input[++i]);
		}
		else if (input[i] == "root" && (i + 1) < input.size() && flagLoc)
		{
			if (!server.getRoot().empty())
				throw  ErrorException("Root is duplicated");
			server.setRoot(input[++i]);
		}
		else if (input[i] == "error_page" && (i + 1) < input.size() && flagLoc)
		{
			while (++i < input.size())
			{
				error_codes.push_back(input[i]);
				if (input[i].find(';') != std::string::npos)
					break ;
				if (i + 1 >= input.size())
					throw ErrorException("Wrong character out of server scope{}");
			}
		}
		else if (input[i] == "client_max_body_size" && (i + 1) < input.size() && flagLoc)
		{
			if (flagMaxSize)
				throw  ErrorException("Client_max_body_size is duplicated");
			server.setClientMaxBodySize(input[++i]);
			flagMaxSize = true;
		}
		else if (input[i] == "server_name" && (i + 1) < input.size() && flagLoc)
		{
			if (!server.getServerName().empty())
				throw  ErrorException("Server_name is duplicated");
			server.setServerName(input[++i]);
		}
		else if (input[i] == "index" && (i + 1) < input.size() && flagLoc)
		{
			if (!server.getIndex().empty())
				throw  ErrorException("Index is duplicated");
			server.setIndex(input[++i]);
		}
		else if (input[i] == "autoindex" && (i + 1) < input.size() && flagLoc)
		{
			if (flagAutoIndex)
				throw ErrorException("Autoindex of server is duplicated");
			server.setAutoindex(input[++i]);
			flagAutoIndex = true;
		}
		else if (input[i] == "location" && (i + 1) < input.size())
		{
			std::string	path;
			i++;
			if (input[i] == "{" || input[i] == "}")
				throw  ErrorException("Wrong character in location block.");
			path = input[i];
			std::vector<std::string> params;
			if (input[++i] != "{")
				throw  ErrorException("Wrong character in location block.");
			i++;
			while (i < input.size() && input[i] != "}")
				params.push_back(input[i++]);
			server.setLocation(path, params);
			if (i < input.size() && input[i] != "}")
				throw  ErrorException("Wrong character in server scope{}");
			flagLoc = 0;
		}
		else if (input[i] != "}" && input[i] != "{")
		{
			if (!flagLoc)
				throw  ErrorException("Parameters after location.");
			else
				throw  ErrorException("Unsupported directive.");
		}
	}
	if (server.getRoot().empty())
		throw ErrorException("Root from config file not found.");
	if (server.getPort() == 0)
		server.setPort("8080;");
	if (server.getHost() == "")
		server.setHost("localhost;");
	if (server.getIndex().empty())
		server.setIndex("index.html;");
	if (ConfigFile::fileExistsandReadable(server.getRoot() + "/" + server.getIndex()) < 0)
		throw ErrorException("Index from config file not found or unreadable.");
	if (server.checkDupLocations())
		throw ErrorException("Location block is duplicated.");
	if (!server.getPort())
		throw ErrorException("Port not found.");
	server.setErrorPages(error_codes);
	if (!server.isValidErrorPages())
		throw ErrorException("Incorrect path for error page or number of error");
	
}

// fonction qui vérifie si 2 blocks Server ont le même server name, host et port
void ConfigParser::checkDupServers(void)
{
	std::vector<ServerConfig>::iterator it1;
	std::vector<ServerConfig>::iterator it2;

	for (it1 = this->_servers.begin(); it1 != this->_servers.end() - 1; it1++)
	{
		for (it2 = it1 + 1; it2 != this->_servers.end(); it2++)
		{
			if (it1->getPort() == it2->getPort() && it1->getHost() == it2->getHost() && it1->getServerName() == it2->getServerName())
				throw ErrorException("Failed server validation. Server block cannot have same server_name, host and port.");
		}
	}
}

std::vector<ServerConfig> ConfigParser::getServers() const
{
	return (this->_servers);
}
