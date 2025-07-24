#include "../include/webserv.hpp"

HttpRequest::HttpRequest()
{
	this->method = "";
	this->uri = "";
	this->body = "";
	this->version = "";
	this->error = -1;
}

HttpRequest::HttpRequest(const HttpRequest &original)
{
	this->method = original.method;
	this->uri = original.uri;
	this->version = original.version;
	this->headers = original.headers;
	this->body = original.body;
	this->error = original.error;
}

HttpRequest		&HttpRequest::operator=(const HttpRequest &original)
{
	if (this != &original)
	{
		this->method = original.method;
		this->uri = original.uri;
		this->version = original.version;
		this->headers = original.headers;
		this->body = original.body;
		this->error = original.error;
	}
	return (*this);
}

HttpRequest::~HttpRequest()
{

}
