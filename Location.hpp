/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmirlico <bmirlico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 23:26:05 by bmirlico          #+#    #+#             */
/*   Updated: 2024/05/20 04:29:21 by bmirlico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "ConfigFile.hpp"

class Location
{
	private:
		std::string					_pathLoc;
		std::string					_rootLoc;
		bool						_autoIndexLoc;
		std::string					_indexLoc;
		std::vector<int>			_methods; // GET + POST + DELETE (+ PUT)
		std::string					_return;
		std::string					_alias;
		std::vector<std::string>	_cgiInterpreter;
		std::vector<std::string>	_cgiExt;
		unsigned long				_clientMaxBodySizeLoc;

	public:
		std::map<std::string, std::string> _extensions;

		Location(void);
		Location(const Location &cpy);
		Location &operator=(const Location &rhs);
		~Location(void);

		void setPath(std::string pathLoc);
		void setRootLocation(std::string rootLoc);
		void setMethods(std::vector<std::string> methods);
		void setAutoindex(std::string autoIndexLoc);
		void setIndexLocation(std::string indexLoc);
		void setReturn(std::string input);
		void setAlias(std::string input);
		void setCgiInterpreter(std::vector<std::string> path);
		void setCgiExtension(std::vector<std::string> extension);
		void setMaxBodySizeLoc(std::string input);

		const std::string &getPath() const;
		const std::string &getRootLocation() const;
		const std::vector<int> &getMethods() const;
		const bool &getAutoIndexLoc() const;
		const std::string &getIndexLocation() const;
		const std::string &getReturn() const;
		const std::string &getAlias() const;
		const std::vector<std::string> &getCgiInterpreter() const;
		const std::vector<std::string> &getCgiExtension() const;
		const std::map<std::string, std::string> &getExtensionPath() const;
		const unsigned long &getMaxBodySizeLoc() const;
		public:
		class ErrorException : public std::exception
		{
			private:
				std::string _message;
			public:
				ErrorException(std::string message) throw()
				{
					_message = "LOCATION CONFIG ERROR: " + message;
				}
				virtual const char* what() const throw()
				{
					return (_message.c_str());
				}
				virtual ~ErrorException() throw() {}
		};
};

#endif