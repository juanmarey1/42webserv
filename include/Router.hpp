#pragma once

#include "webserv.hpp"

class Router
{
	public:
		Router();
		Router(const Router &original);
		Router	&operator=(const Router &original);
		~Router();

		std::pair<ServerConfig, LocationConfig>	route(const HttpRequest &request, const std::vector<ServerConfig> &servers);	//Chooses the right ServerConfig block and the best LocationConfig block. This matched config is passed to ResponseBuilder or CGIHandler

};
