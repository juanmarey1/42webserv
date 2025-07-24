#include "../include/webserv.hpp"

LocationConfig::LocationConfig()
{
	this->locationPath = "";
	this->root = "";
	this->autoindex = false;
	this->cgi_extension = "";
	this->cgi_path = "";
	this->upload_enable = false;
	this->upload_store = "";
	this->exactMatch = false;
	this->client_max_body_size = -1;
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
	this->exactMatch = original.exactMatch;
	this->client_max_body_size = original.client_max_body_size;
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
		this->exactMatch = original.exactMatch;
		this->client_max_body_size = original.client_max_body_size;
	}
	return (*this);
}

LocationConfig::~LocationConfig()
{

}

bool		LocationConfig::operator==(const LocationConfig &original) const
{
	return (this->allowed_methods == original.allowed_methods && this->autoindex == original.autoindex && this->cgi_extension == original.cgi_extension && this->cgi_path == original.cgi_path && this->index == original.index && this->locationPath == original.locationPath && this->returnDirective == original.returnDirective && this->root == original.root && this->upload_enable == original.upload_enable && this->upload_store == original.upload_store && this->client_max_body_size == original.client_max_body_size);
}
