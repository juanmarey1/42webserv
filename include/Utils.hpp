#pragma once

#include "webserv.hpp"

namespace	Utils
{
	bool						ends_with(const std::string& fullString, const std::string& ending);
	std::string					ft_trim(const std::string &str);
	std::string					urlDecode(const std::string &encode);
	std::string					getFileExtension(const std::string &path);
	std::string 				statusText(int code);
	bool 						fileExists(const std::string &path);
	bool                        isDirectory(const std::string& path);
	std::vector<std::string> 	splitBySpaces(const std::string& line);
	size_t 						getNextLine(const std::string& buffer, size_t& pos, std::string& line);
	size_t 						firstDifference(const std::string& s1, const std::string& s2);
	std::string 				toLowerAlphaOnly(const std::string& str);
	bool        				endsWith(const std::string &str, const std::string &suffix);
};
