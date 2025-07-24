#include "../include/webserv.hpp"

Router::Router()
{

}

Router::Router(const Router &original)
{
	*this = original;
}

Router		&Router::operator=(const Router &original)
{
	if (this != &original)
	{
		;
	}
	return (*this);
}

Router::~Router()
{

}

std::pair<ServerConfig, LocationConfig>		Router::route(const HttpRequest &request, const std::vector<ServerConfig> &servers)
{

	std::pair<ServerConfig, LocationConfig>		config;

	//Si solo hay un servidor en la lista, usamos ese
	if (servers.size() == 1)
	{
		config.first = servers[0];
	}
	else 
	{
		//Si no, usamos el que server_name coincida con Host
		std::string		hostName;

		hostName = Utils::toLowerAlphaOnly(request.headers.find("Host")->second);
		size_t i;
	
		for (i = 0; i < servers.size(); i++)
		{
			if (Utils::toLowerAlphaOnly(servers[i].server_name) == hostName)
			{
				config.first = servers[i];
				break;
			}
			if (servers[i].server_name[0] == '*')
			{
				std::string	wildName = servers[i].server_name.substr(1);
				if (Utils::ends_with(Utils::toLowerAlphaOnly(hostName), Utils::toLowerAlphaOnly(wildName)))
				{
					config.first = servers[i];
					break;
				}
			}
		}
		//Si no coincide ningun server_name con Host, elegimos el primero por defecto
		if (i == servers.size())
		{
			config.first = servers[0];
		}
	}

	//Ahora escogemos localizacion
	std::vector<LocationConfig>	locations = config.first.locations;
	std::string 				uri = request.uri;

	//We first search exact matches
	size_t j;
	for (j = 0; j < locations.size(); j++)
	{
		if (uri == locations[j].locationPath && locations[j].exactMatch)
		{
			config.second = locations[j];
			return (config);
		}
	}
	//Then we search for prefix matches (the largest matching, wins)
	size_t len = 0;
	for (j = 0; j < locations.size(); j++)
	{
		if (uri.find(locations[j].locationPath) != std::string::npos && !locations[j].exactMatch)
		{
			if (locations[j].locationPath.size() > len)
			{
				len = locations[j].locationPath.size();
				config.second = locations[j];
			}
		}
		if (len == 0 && j + 1 == locations.size())
		{
			for (size_t k = 0; k < locations.size(); k++)
			{
				if (locations[k].locationPath == "/")
				{
					config.second = locations[k];
					return (config);
				}
			}
			config.second = locations[0];
			return (config); 
			
		}
	}
	return (config);
}
