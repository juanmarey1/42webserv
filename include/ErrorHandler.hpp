#pragma once

#include "webserv.hpp"

class ErrorHandler
{
	public:
		ErrorHandler();
		ErrorHandler(const ErrorHandler &original);
		ErrorHandler	&operator=(const ErrorHandler &original);
		~ErrorHandler();

		static HttpResponse	generateHttpResponse(int status_code, const ServerConfig &config);	//Checks if a custom error page is configured. It also sets correct status line and headers for error response

};
