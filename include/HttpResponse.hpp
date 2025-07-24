#pragma once

#include "webserv.hpp"


class HttpResponse
{
	public:
		std::string			build();				//Constructs the full HTTP response string

		HttpResponse();
		HttpResponse(const HttpResponse &original);
		HttpResponse	&operator=(const HttpResponse &original);
		~HttpResponse();

		int									status_code;
		std::string							status_text;
		std::map<std::string, std::string>	headers;
		std::string							body;
		
};
