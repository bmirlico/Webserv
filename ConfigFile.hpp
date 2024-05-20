/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmirlico <bmirlico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 17:19:49 by bmirlico          #+#    #+#             */
/*   Updated: 2024/05/20 04:10:32 by bmirlico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <ctime>
#include <stdexcept>

#define MAX_BODY_LENGTH 2000000

class ConfigFile
{
	private:
		std::string _filePath;
	public:
		ConfigFile(void);
		ConfigFile(const std::string path);
		ConfigFile(const ConfigFile &cpy);
		ConfigFile& operator=(const ConfigFile &rhs);
		~ConfigFile(void);
		static int getTypeFilePath(std::string const path);
		static int fileExistsandReadable(std::string const path);
		static int filePathExistsandReadable(std::string const path, std::string const index);
		std::string readConfigFile(std::string path);
		std::string getFilePath(void) const;
};

#endif