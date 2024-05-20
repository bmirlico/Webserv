/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmirlico <bmirlico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 23:25:23 by bmirlico          #+#    #+#             */
/*   Updated: 2024/05/20 04:29:08 by bmirlico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(void)
{
	this->_pathLoc = "";
	this->_rootLoc = "";
	this->_autoIndexLoc = false;
	this->_indexLoc = "";
	this->_return = "";
	this->_alias = "";
	this->_clientMaxBodySizeLoc = MAX_BODY_LENGTH;
	this->_methods.reserve(5);
	this->_methods.push_back(0);
	this->_methods.push_back(0);
	this->_methods.push_back(0);
	this->_methods.push_back(0);
	this->_methods.push_back(0);
}

Location::Location(const Location &cpy)
{
	*this = cpy;
}

Location &Location::operator=(const Location &rhs)
{
	if (this != &rhs)
	{
		this->_pathLoc = rhs._pathLoc;
		this->_rootLoc = rhs._rootLoc;
		this->_autoIndexLoc = rhs._autoIndexLoc;
		this->_indexLoc = rhs._indexLoc;
		this->_cgiInterpreter = rhs._cgiInterpreter;
		this->_cgiExt = rhs._cgiExt;
		this->_return = rhs._return;
		this->_alias = rhs._alias;
		this->_methods = rhs._methods;
		this->_extensions = rhs._extensions;
		this->_clientMaxBodySizeLoc = rhs._clientMaxBodySizeLoc;
    }
	return (*this);
}

Location::~Location() { }

/* set functions */
void Location::setPath(std::string pathLoc)
{
	this->_pathLoc = pathLoc;
}

void Location::setRootLocation(std::string rootLoc)
{
	if (ConfigFile::getTypeFilePath(rootLoc) != 2)
		throw ErrorException("Invalid rood for the location block.");
	this->_rootLoc = rootLoc;
}

void Location::setMethods(std::vector<std::string> methods)
{
	this->_methods[0] = 0;
	this->_methods[1] = 0;
	this->_methods[2] = 0;
	this->_methods[3] = 0;
	this->_methods[4] = 0;

	for (size_t i = 0; i < methods.size(); i++)
	{
		if (methods[i] == "GET")
			this->_methods[0] = 1;
		else if (methods[i] == "POST")
			this->_methods[1] = 1;
		else if (methods[i] == "DELETE")
			this->_methods[2] = 1;
		else if (methods[i] == "PUT")
			this->_methods[3] = 1;
		else
			throw ErrorException("Method not supported for: " + methods[i]);
	}
}

void Location::setAutoindex(std::string autoIndexLoc)
{
	if (autoIndexLoc == "on" || autoIndexLoc == "off")
		this->_autoIndexLoc = (autoIndexLoc == "on");
	else
		throw ErrorException("Wrong autoindex.");
}

void Location::setIndexLocation(std::string indexLoc)
{
	this->_indexLoc = indexLoc;
}

void Location::setReturn(std::string input)
{
	this->_return = input;
}

void Location::setAlias(std::string input)
{
	this->_alias = input;
}

void Location::setCgiInterpreter(std::vector<std::string> path)
{
	this->_cgiInterpreter = path;
}

void Location::setCgiExtension(std::vector<std::string> extension)
{
	this->_cgiExt = extension;
}

void Location::setMaxBodySizeLoc(std::string input)
{
	unsigned long body_size;
	
	body_size = 0;
	for (size_t i = 0; i < input.length(); i++)
	{
		if (!std::isdigit(input[i]))
			throw ErrorException("Client_max_body_size can only be an integer.");
	}
	body_size = std::strtol(input.c_str(), NULL, 10);;
	this->_clientMaxBodySizeLoc = body_size;
}

// Getters
const std::string &Location::getPath() const
{
	return (this->_pathLoc);
}

const std::string &Location::getRootLocation() const
{
	return (this->_rootLoc);
}

const std::string &Location::getIndexLocation() const
{
	return (this->_indexLoc);
}

const std::vector<int> &Location::getMethods() const
{
	return (this->_methods);
}

const std::vector<std::string> &Location::getCgiInterpreter() const
{
	return (this->_cgiInterpreter);
}

const std::vector<std::string> &Location::getCgiExtension() const
{
	return (this->_cgiExt);
}

const bool &Location::getAutoIndexLoc() const
{
	return (this->_autoIndexLoc);
}

const std::string &Location::getReturn() const
{
	return (this->_return);
}

const std::string &Location::getAlias() const
{
	return (this->_alias);
}

const std::map<std::string, std::string> &Location::getExtensionPath() const
{
	return (this->_extensions);
}

const unsigned long &Location::getMaxBodySizeLoc() const
{
	return (this->_clientMaxBodySizeLoc);
}
