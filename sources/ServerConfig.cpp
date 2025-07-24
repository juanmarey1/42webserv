#include "../include/webserv.hpp"

ServerConfig::ServerConfig()
{
	this->ip = "";
	this->port = -1;
	this->server_name = "";
	this->root = "";
	this->client_max_body_size = -1;
	this->autoindex = false;
}

ServerConfig::ServerConfig(const ServerConfig &original)
{
	this->locations = original.locations;
	this->ip = original.ip;
	this->port = original.port;
	this->server_name = original.server_name;
	this->root = original.root;
	this->error_page = original.error_page;
	this->client_max_body_size = original.client_max_body_size;
	this->index = original.index;
	this->autoindex = original.autoindex;
	
}

ServerConfig		&ServerConfig::operator=(const ServerConfig &original)
{
	if (this != &original)
	{
		this->locations = original.locations;
		this->ip = original.ip;
		this->port = original.port;
		this->server_name = original.server_name;
		this->root = original.root;
		this->error_page = original.error_page;
		this->client_max_body_size = original.client_max_body_size;
		this->index = original.index;
		this->autoindex = original.autoindex;
	}
	return (*this);
}

ServerConfig::~ServerConfig()
{

}

bool		ServerConfig::operator==(const ServerConfig &original)
{
	return (original.client_max_body_size == this->client_max_body_size && original.error_page == this->error_page && this->index == original.index && this->ip == original.ip && this->locations == original.locations && this->port == original.port && this->root == original.root && this->server_name == original.server_name && this->autoindex == original.autoindex);
}

void		ServerConfig::check(const ServerConfig &server, LocationConfig &location)
{
	if (location.root.empty())
	{
		location.root = server.root;
	}
	if (location.index.empty())
	{
		location.index = server.index;
	}
	if ((location.upload_enable == true && location.upload_store.empty()) || (location.upload_enable == false && !location.upload_store.empty()))
	{
		throw MissingDirectiveExcept();
	}
	if (location.allowed_methods.empty())
	{
		location.allowed_methods.push_back("GET");
	}
	if (location.cgi_path.empty())
	{
		location.cgi_path = "";
	}
	if (location.cgi_extension.empty())
	{
		location.cgi_extension = "";
	}
	if (location.client_max_body_size == -1)
	{
		location.client_max_body_size = server.client_max_body_size;
	}
	if (!location.autoindex)
	{
		location.autoindex = server.autoindex;
	}
}


void 			ServerConfig::check()
{
	if (this->ip.empty())
	{
		this->ip = "0.0.0.0";
	}
	if (this->port == -1)
	{
		this->port = 80;
	}
	if (this->root.empty())
	{
		this->root = "";
	}
	if (this->client_max_body_size == -1)
	{
		this->client_max_body_size = 1 * 1024 * 1024;
	}
	if (this->index.empty())
	{
		this->index.push_back("index.html");
		this->index.push_back("index.htm");
	}
}
