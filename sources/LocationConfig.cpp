#include "../include/webserv.hpp"

LocationConfig::LocationConfig()
{
	this->locationPath = "";
	this->root = "";
	this->autoindex = false;
	this->index = "";
	this->cgi_extension = "";
	this->cgi_path = "";
	this->upload_enable = false;
	this->upload_store = "";
}

LocationConfig::LocationConfig(const LocationConfig &original)
{
	this->locationPath = original.locationPath;
	this->root = original.root;
	this->autoindex = original.autoindex;
	this->allowed_methods = original.allowed_methods;
	this->index = original.index;
	this->returnDirective = original.returnDirective;
	this->cgi_extension = original.cgi_extension;
	this->cgi_path = original.cgi_path;
	this->upload_enable = original.upload_enable;
	this->upload_store = original.upload_store;
}

LocationConfig		&LocationConfig::operator=(const LocationConfig &original)
{
	if (this != &original)
	{
		this->locationPath = original.locationPath;
		this->root = original.root;
		this->autoindex = original.autoindex;
		this->allowed_methods = original.allowed_methods;
		this->index = original.index;
		this->returnDirective = original.returnDirective;
		this->cgi_extension = original.cgi_extension;
		this->cgi_path = original.cgi_path;
		this->upload_enable = original.upload_enable;
		this->upload_store = original.upload_store;
	}
	return (*this);
}

LocationConfig::~LocationConfig()
{

}

void		LocationConfig::check(const ServerConfig &server)
{
	if (this->root.empty())
	{
		this->root = server.root;
	}
	if (this->index.empty())
	{
		this->index = server.index;
	}
}
