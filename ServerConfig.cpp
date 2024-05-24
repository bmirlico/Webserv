/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmirlico <bmirlico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 19:12:28 by bmirlico          #+#    #+#             */
/*   Updated: 2024/05/24 16:31:34 by bmirlico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig(void)
{
	this->_port = 0;
	this->_host = "";
	this->_serverName = "";
	this->_root = "";
	this->_clientMaxBodySize = MAX_BODY_LENGTH;
	this->_index = "";
	this->_autoIndex = false;
	this->initErrorPages();
}

ServerConfig::~ServerConfig() {}

ServerConfig::ServerConfig(const ServerConfig &cpy)
{
	*this = cpy;
}

ServerConfig &ServerConfig::operator=(const ServerConfig & rhs)
{
	if (this != &rhs)
	{
		this->_serverName = rhs._serverName;
		this->_root = rhs._root;
		this->_port = rhs._port;
		this->_host = rhs._host;
		this->_clientMaxBodySize = rhs._clientMaxBodySize;
		this->_index = rhs._index;
		this->_errorPages = rhs._errorPages;
		this->_locations = rhs._locations;
		this->_autoIndex = rhs._autoIndex;
	}
	return (*this);
}

void ServerConfig::initErrorPages(void)
{
	// _errorPages[301] = "";
	// _errorPages[302] = "";
	// _errorPages[400] = "";
	// _errorPages[401] = "";
	// _errorPages[402] = "";
	// _errorPages[403] = "";
	// _errorPages[404] = "";
	// _errorPages[405] = "";
	// _errorPages[406] = "";
	// _errorPages[500] = "";
	// _errorPages[501] = "";
	// _errorPages[502] = "";
	// _errorPages[503] = "";
	// _errorPages[504] = "";
	// _errorPages[505] = "";
}

void ServerConfig::checkSemiColon(std::string &input)
{
	size_t pos = input.rfind(';');
	if (pos != input.size() - 1)
		throw ErrorException("Invalid format: each directive should be finished with ;.");
	input.erase(pos);
}

void ServerConfig::setServerName(std::string serverName)
{
	checkSemiColon(serverName);
	this->_serverName = serverName;
}

void ServerConfig::setHost(std::string host)
{
	checkSemiColon(host);
	if (host == "localhost")
		host = "127.0.0.1";
	if (!isValidHost(host))
		throw ErrorException("Wrong syntax: host");
	this->_host = host;
}

bool ServerConfig::isInteger(std::string& s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
		return (false);
    char* p;
    std::strtol(s.c_str(), &p, 10);
    return (*p == 0);
}

bool ServerConfig::isValidHost(std::string host)
{
	std::istringstream ss(host);
    std::string token;
    int segments = 0;

    while (std::getline(ss, token, '.'))
	{
        if (!isInteger(token))
			return (false);
        int num = std::atoi(token.c_str());
        if (num < 0 || num > 255)
			return (false);
        segments++;
    }
    return (segments == 4);
}

void ServerConfig::setRoot(std::string root)
{
	checkSemiColon(root);
	if (ConfigFile::getTypeFilePath(root) == 2)
		this->_root = root;
	else
		throw ErrorException("Wrong syntax: root");
}

void ServerConfig::setPort(std::string input)
{
	unsigned int port;
	
	port = 0;
	checkSemiColon(input);
	for (size_t i = 0; i < input.length(); i++)
	{
		if (!std::isdigit(input[i]))
			throw ErrorException("Port can only be an integer.");
	}
	port = std::strtol(input.c_str(), NULL, 10);
	if (port < 1 || port > 65635)
		throw ErrorException("Invalid port number.");
	this->_port = (uint16_t)port;
}

void ServerConfig::setClientMaxBodySize(std::string input)
{
	unsigned long body_size;
	
	body_size = 0;
	checkSemiColon(input);
	for (size_t i = 0; i < input.length(); i++)
	{
		if (!std::isdigit(input[i]))
			throw ErrorException("Client_max_body_size can only be an integer.");
	}
	body_size = std::strtol(input.c_str(), NULL, 10);;
	this->_clientMaxBodySize = body_size;
}

void ServerConfig::setIndex(std::string index)
{
	checkSemiColon(index);
	this->_index = index;
}

void ServerConfig::setAutoindex(std::string autoindex)
{
	checkSemiColon(autoindex);
	if (autoindex != "on" && autoindex != "off")
		throw ErrorException("Autoindex can only take on or off as values.");
	if (autoindex == "on")
		this->_autoIndex = true;
}

void ServerConfig::setErrorPages(std::vector<std::string> &errorPages)
{
	if (errorPages.empty())
		return ;
	// if (errorPages.size() % 2 != 0)
	// 	throw ErrorException ("Error page initialization failed");
	std::string errorPagePath = errorPages.back();
	checkSemiColon(errorPagePath);
	if (ConfigFile::getTypeFilePath((this->_root + errorPagePath)) != 1)
			throw ErrorException ("Incorrect path for error page file: " + errorPagePath);
	if (ConfigFile::filePathExistsandReadable(this->_root + "/", errorPagePath) != 1)
			throw ErrorException ("Error page file :" + errorPagePath + " is not accessible");
	for (size_t i = 0; i < errorPages.size() - 1; i++)
	{
		for (size_t j = 0; j < errorPages[i].size(); j++) {
			if (!std::isdigit(errorPages[i][j]))
				throw ErrorException("Error code should only contain digits.");
		}
		if (errorPages[i].size() != 3)
			throw ErrorException("Error code is a 3-digit number.");
		int codeError = std::strtol(errorPages[i].c_str(), NULL, 10);
		if (statusCodeString(codeError)  == "Undefined" || codeError < 400)
			throw ErrorException ("Incorrect error code: " + errorPages[i]);
		std::map<int, std::string>::iterator it = this->_errorPages.find(codeError);
		if (it != _errorPages.end())
			this->_errorPages[codeError] = errorPagePath;
		else
			this->_errorPages.insert(std::make_pair(codeError, errorPagePath));
	}
}

std::string ServerConfig::statusCodeString(int statusCode)
{
    switch (statusCode)
    {
        case 100:
            return "Continue";
        case 101:
            return "Switching Protocol";
        case 200:
            return "OK";
        case 201:
            return "Created";
        case 202:
            return "Accepted";
        case 203:
            return "Non-Authoritative Information";
        case 204:
            return "No Content";
        case 205:
            return "Reset Content";
        case 206:
            return "Partial Content";
        case 300:
            return "Multiple Choice";
        case 301:
            return "Moved Permanently";
        case 302:
            return "Moved Temporarily";
        case 303:
            return "See Other";
        case 304:
            return "Not Modified";
        case 307:
            return "Temporary Redirect";
        case 308:
            return "Permanent Redirect";
        case 400:
            return "Bad Request";
        case 401:
            return "Unauthorized";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 405:
            return "Method Not Allowed";
        case 406:
            return "Not Acceptable";
        case 407:
            return "Proxy Authentication Required";
        case 408:
            return "Request Timeout";
        case 409:
            return "Conflict";
        case 410:
            return "Gone";
        case 411:
            return "Length Required";
        case 412:
            return "Precondition Failed";
        case 413:
            return "Payload Too Large";
        case 414:
            return "URI Too Long";
        case 415:
            return "Unsupported Media Type";
        case 416:
            return "Requested Range Not Satisfiable";
        case 417:
            return "Expectation Failed";
        case 418:
            return "I'm a teapot";
        case 421:
            return "Misdirected Request";
        case 425:
            return "Too Early";
        case 426:
            return "Upgrade Required";
        case 428:
            return "Precondition Required";
        case 429:
            return "Too Many Requests";
        case 431:
            return "Request Header Fields Too Large";
        case 451:
            return "Unavailable for Legal Reasons";
        case 500:
            return "Internal Server Error";
        case 501:
            return "Not Implemented";
        case 502:
            return "Bad Gateway";
        case 503:
            return "Service Unavailable";
        case 504:
            return "Gateway Timeout";
        case 505:
            return "HTTP Version Not Supported";
        case 506:
            return "Variant Also Negotiates";
        case 507:
            return "Insufficient Storage";
        case 510:
            return "Not Extended";
        case 511:
            return "Network Authentication Required";
        default:
            return "Undefined";
        }
}

bool ServerConfig::isValidErrorPages(void)
{
	std::map<int, std::string>::const_iterator it;
	for (it = this->_errorPages.begin(); it != this->_errorPages.end(); it++)
	{
		if (it->first < 100 || it->first > 599)
			return (false);
		if (ConfigFile::fileExistsandReadable(this->getRoot() + "/" + it->second) < 0)
			return (false);
	}
	return (true);
}

std::string ServerConfig::intToString(int number)
{
    std::ostringstream oss;
    oss << number;
    return oss.str();
}

void ServerConfig::setLocation(std::string path, std::vector<std::string> input)
{
	Location newLocation;
	std::vector<std::string> methods;
	bool flag_methods = false;
	bool flagAutoIndex = false;
	bool flagMaxSize = false;
	int valid;

	newLocation.setPath(path);
	for (size_t i = 0; i < input.size(); i++)
	{
		if (input[i] == "root" && (i + 1) < input.size())
		{
			if (!newLocation.getRootLocation().empty())
				throw ErrorException("Root of location is duplicated.");
			checkSemiColon(input[++i]);
			if (ConfigFile::getTypeFilePath(input[i]) != 2)
				throw ErrorException("Invalid root of location.");
			newLocation.setRootLocation(input[i]);
		}
		else if ((input[i] == "allow_methods" || input[i] == "methods") && (i + 1) < input.size())
		{
			if (flag_methods)
				throw ErrorException("Allow_methods of location is duplicated");
			std::vector<std::string> methods;
			while (++i < input.size())
			{
				if (input[i].find(";") != std::string::npos)
				{
					checkSemiColon(input[i]);
					methods.push_back(input[i]);
					break ;
				}
				else
				{
					methods.push_back(input[i]);
					if (i + 1 >= input.size())
						throw ErrorException("Invalid method, out of location scope.");
				}
			}
			newLocation.setMethods(methods);
			flag_methods = true;
		}
		else if (input[i] == "autoindex" && (i + 1) < input.size())
		{
			if (path == "/cgi-bin")
				throw ErrorException("Autoindex not allowed for CGI location block.");
			if (flagAutoIndex)
				throw ErrorException("Autoindex of location is duplicated.");
			checkSemiColon(input[++i]);
			newLocation.setAutoindex(input[i]);
			flagAutoIndex = true;
		}
		else if (input[i] == "index" && (i + 1) < input.size())
		{
			if (!newLocation.getIndexLocation().empty())
				throw ErrorException("Index of location is duplicated");
			checkSemiColon(input[++i]);
			newLocation.setIndexLocation(input[i]);
		}
		else if (input[i] == "return" && (i + 1) < input.size())
		{
			if (path == "/cgi-bin")
				throw ErrorException("Return not allowed for CGI location block");
			if (!newLocation.getReturn().empty())
				throw ErrorException("Return of location is duplicated");
			checkSemiColon(input[++i]);
			newLocation.setReturn(input[i]);
		}
		else if (input[i] == "alias" && (i + 1) < input.size())
		{
			if (path == "/cgi-bin")
				throw ErrorException("Alias not allowed for CGI");
			if (!newLocation.getAlias().empty())
				throw ErrorException("Alias of location is duplicated");
			checkSemiColon(input[++i]);
			newLocation.setAlias(input[i]);
		}
		else if (input[i] == "cgi_ext" && (i + 1) < input.size())
		{
			std::vector<std::string> extension;
			while (++i < input.size())
			{
				if (input[i].find(";") != std::string::npos)
				{
					checkSemiColon(input[i]);
					extension.push_back(input[i]);
					break ;
				}
				else
				{
					extension.push_back(input[i]);
					if (i + 1 >= input.size())
						throw ErrorException("Invalid CGI extension, out of location scope.");
				}
			}
			newLocation.setCgiExtension(extension);
		}
		else if (input[i] == "cgi_interpreter" && (i + 1) < input.size())
		{
			std::vector<std::string> path;
			while (++i < input.size())
			{
				if (input[i].find(";") != std::string::npos)
				{
					checkSemiColon(input[i]);
					path.push_back(input[i]);
					break ;
				}
				else
				{
					path.push_back(input[i]);
					if (i + 1 >= input.size())
						throw ErrorException("Invalid CGI interpreter, out of location scope.");
				}
				if (input[i].find("/python") == std::string::npos && input[i].find("/bash") == std::string::npos)
					throw ErrorException("cgi_interpreter path is invalid.");
			}
			newLocation.setCgiInterpreter(path);
		}
		else if (input[i] == "client_max_body_size" && (i + 1) < input.size())
		{
			if (flagMaxSize)
				throw ErrorException("Maxbody_size of location is duplicated");
			checkSemiColon(input[++i]);
			newLocation.setMaxBodySizeLoc(input[i]);
			flagMaxSize= true;
		}
		else if (i < input.size())
			throw ErrorException("Invalid parameter in a location bloc."); // ? to check avec un cas particulier
	}
	if (newLocation.getPath() != "/cgi-bin" && newLocation.getRootLocation().empty()) // s'il n'y pas de root dans le block location on met le root du server
		newLocation.setRootLocation(this->_root); // on recheck plus tard si root du server existe, s'il n'existe pas => error
	if (newLocation.getIndexLocation().empty())
		newLocation.setIndexLocation(this->_index);
	if (this->_index.empty())
		throw ErrorException("No default index file, or present after location block.");
	if (!flagMaxSize)
		newLocation.setMaxBodySizeLoc(intToString(this->_clientMaxBodySize));
	std::vector<std::string> methodsDefault;
	if (newLocation.getPath() == "/cgi-bin")
		newLocation.setMethods(methodsDefault); // met les méthodes du CGI par défaut GET et POST
	else
		newLocation.setMethods(methodsDefault); // met la méthode GET par défaut pour les blocs location classique
	valid = isValidLocation(newLocation);
	if (valid == 1)
		throw ErrorException("Failed CGI validation.");
	else if (valid == 2)
		throw ErrorException("Failed path in location validation.");
	else if (valid == 3)
		throw ErrorException("Failed redirection file in location block.");
	else if (valid == 4)
		throw ErrorException("Failed alias file in location block.");
	else if (valid == 5)
		throw ErrorException("Failed route in location block.");
	else if (valid == 6)
		throw ErrorException("Failed index in location block.");
	this->_locations.push_back(newLocation);
}

// fonction qui check les paramètre du block location
// TO CHECK => isvalidlocation + tester toutes les permissions de fichier
int ServerConfig::isValidLocation(Location &location) const
{
	if (location.getPath() == "/cgi-bin")
	{
		// std::cout << location.getRootLocation() << std::endl;
		// std::cout << location.getIndexLocation() << std::endl;
		// for (size_t i = 0; i < location.getCgiInterpreter().size(); i++)
		// 	std::cout << location.getCgiInterpreter()[i] << std::endl;
		// for (size_t i = 0; i < location.getCgiExtension().size(); i++)
		// 	std::cout << location.getCgiExtension()[i] << std::endl;
		if (location.getCgiInterpreter().empty() || location.getCgiExtension().empty() || location.getRootLocation().empty() || location.getIndexLocation().empty())
			return (1);
		if (ConfigFile::fileExistsandReadable(this->_root + "/" + location.getIndexLocation()) < 0)
		{
			std::string path = location.getRootLocation() + location.getPath() + "/" + location.getIndexLocation();
			if (ConfigFile::fileExistsandReadable(path) < 0)
				return (1);
		}
		if (location.getCgiInterpreter().size() != location.getCgiExtension().size())
			return (1);
		std::vector<std::string>::const_iterator it;
		for (it = location.getCgiInterpreter().begin(); it != location.getCgiInterpreter().end(); ++it)
		{
			if (ConfigFile::fileExistsandReadable(*it) < 0)
				return (1);
		}
		std::vector<std::string>::const_iterator it_path;
		for (it = location.getCgiExtension().begin(); it != location.getCgiExtension().end(); ++it)
		{
			std::string tmp = *it;
			if (tmp != ".py" && tmp != ".sh")
				return (1);
			for (it_path = location.getCgiInterpreter().begin(); it_path != location.getCgiInterpreter().end(); ++it_path)
			{
				std::string tmp_path = *it_path;
				if (tmp == ".py")
				{
					if (tmp_path.find("python") != std::string::npos)
						location._extensions[".py"] = tmp_path; // besoin d'un getter ?
				}
				else if (tmp == ".sh")
				{
					if (tmp_path.find("bash") != std::string::npos)
						location._extensions[".sh"] = tmp_path;
				}
			}
		}
		if (location.getCgiInterpreter().size() != location.getExtensionPath().size())
			return (1);
	}
	else
	{
		if (location.getPath()[0] != '/')
			return (2);
		if (location.getRootLocation().empty()) {
			location.setRootLocation(this->_root);
		}
		// if (ConfigFile::getTypeFilePath(location.getRootLocation() + location.getPath()) != 2) // on check que le root + path est bien un dir qui existe, est-ce vraiment nécessaire ?
		// 	return (5);
		if (ConfigFile::filePathExistsandReadable(location.getRootLocation() + "/", location.getIndexLocation()) == 0) // on check que l'index file existe
			return (6);
		if (!location.getReturn().empty())
		{
			if (ConfigFile::filePathExistsandReadable(location.getRootLocation() + "/", location.getReturn()) == 0) // on check que le return file existe
				return (3);
			// shouldn't work
		}
		if (!location.getAlias().empty())
		{
			if (ConfigFile::getTypeFilePath(location.getAlias()) != 2) // on check que l'alias est bien un dir qui existe
				return (4);
		}
	}
	return (0);
}

bool ServerConfig::checkDupLocations(void)
{
	if (this->_locations.size() < 2)
		return (false);
	std::vector<Location>::const_iterator it1;
	std::vector<Location>::const_iterator it2;
	for (it1 = this->_locations.begin(); it1 != this->_locations.end() - 1; it1++) {
		for (it2 = it1 + 1; it2 != this->_locations.end(); it2++) {
			if (it1->getPath() == it2->getPath())
				return (true);
		}
	}
	return (false);
}

// Getters

const std::string &ServerConfig::getServerName()
{
	return (this->_serverName);
}

const std::string &ServerConfig::getRoot()
{
	return (this->_root);
}

const bool &ServerConfig::getAutoindex()
{
	return (this->_autoIndex);
}

const std::string &ServerConfig::getHost()
{
	return (this->_host);
}

const uint16_t &ServerConfig::getPort()
{
	return (this->_port);
}

const size_t &ServerConfig::getClientMaxBodySize()
{
	return (this->_clientMaxBodySize);
}

const std::vector<Location> &ServerConfig::getLocations()
{
	return (this->_locations);
}

const std::map<int, std::string> &ServerConfig::getErrorPages()
{
	return (this->_errorPages);
}

const std::string &ServerConfig::getIndex()
{
	return (this->_index);
}