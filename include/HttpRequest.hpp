#pragma once

#include "webserv.hpp"

class HttpRequest
{
	public:
		std::string							method;
		std::string							uri;
		std::string							version;
		std::map<std::string, std::string>	headers;
		std::string							body;
		int									error;

		HttpRequest();
		HttpRequest(const HttpRequest &original);
		HttpRequest		&operator=(const HttpRequest &original);
		~HttpRequest();

};
