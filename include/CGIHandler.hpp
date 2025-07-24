#pragma once

#include "webserv.hpp"

class CGIHandler
{
	private:
		std::vector<std::string>				env;			//Stores the environment variables required by CGI
		std::string								cgiOutput;		//Where the cgiOutput is stored for later parsing

		void				setupEnv(const HttpRequest &request, const LocationConfig &location);	//Prepares CGI environment variables for the execve
		int					forkExecute(const std::string &scriptPath, const HttpRequest &request);								//Runs the CGI script by forking the current process.

	public:
		CGIHandler();
		CGIHandler(const CGIHandler &original);
		CGIHandler	&operator=(const CGIHandler &original);
		~CGIHandler();

		HttpResponse		executeScript(const HttpRequest &request, const LocationConfig &location, const ServerConfig &server);	//Runs CGI script based on incoming request and returns a HttpResponse from the script's output

};
