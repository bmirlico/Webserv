/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmirlico <bmirlico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 23:25:23 by bmirlico          #+#    #+#             */
/*   Updated: 2024/05/23 03:35:59 by bmirlico         ###   ########.fr       */
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
	this->_methods.reserve(3); // pourquoi faire ça ?
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
		throw ErrorException("Invalid root for the location block.");
	this->_rootLoc = rootLoc;
}

void Location::setMethods(std::vector<std::string> methods)
{
	// this->_methods[0] = 0;
	// this->_methods[1] = 0;
	// this->_methods[2] = 0;

	for (size_t i = 0; i < methods.size(); i++)
	{
		if (methods[i] == "GET")
			this->_methods[0] = 1;
		else if (methods[i] == "POST")
			this->_methods[1] = 1;
		else if (methods[i] == "DELETE" && this->_pathLoc != "/cgi-bin")
			this->_methods[2] = 1;
		else
			throw ErrorException("Method not supported: " + methods[i]);
	}
	if (this->_pathLoc == "/cgi-bin" && methods.size() == 0)
	{
		for (size_t i = 0; i < 2; i++)
			this->_methods[i] = 1;
	}
	else if (this->_pathLoc != "/cgi-bin" && methods.size() == 0)
		this->_methods[0] = 1;
}

std::string Location::getPrintMethods(void) const
{
	std::string res;
	if (_methods[2])
	{
		if (!res.empty())
			res.insert(0, ", ");
		res.insert(0, "DELETE");
	}
	if (_methods[1])
	{
		if (!res.empty())
			res.insert(0, ", ");
		res.insert(0, "POST");
	}
	if (_methods[0])
	{
		if (!res.empty())
			res.insert(0, ", ");
		res.insert(0, "GET");
	}
	return (res);
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

std::vector<int> &Location::getMethods(void)
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
