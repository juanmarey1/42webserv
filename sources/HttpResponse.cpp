#include "../include/webserv.hpp"

HttpResponse::HttpResponse()
{
	this->status_code = -1;
	this->status_text = "";
	this->body = "";
}

HttpResponse::HttpResponse(const HttpResponse &original)
{
	this->status_code = original.status_code;
	this->status_text = original.status_text;
	this->headers = original.headers;
	this->body = original.body;
}

HttpResponse		&HttpResponse::operator=(const HttpResponse &original)
{
	if (this != &original)
	{
		this->status_code = original.status_code;
		this->status_text = original.status_text;
		this->headers = original.headers;
		this->body = original.body;
	}
	return (*this);
}

HttpResponse::~HttpResponse()
{

}


//functions
std::string			HttpResponse::build()
{
	std::string			buffer;
	std::ostringstream	oss;

	oss << this->status_code;
	std::string code = oss.str();

	//Create response line
	buffer.append("HTTP/1.1 " + code + " " + this->status_text + "\r\n");
	//Headers: 
	for (std::map<std::string,  std::string>::iterator it = this->headers.begin(); it != this->headers.end(); it++)
	{
		buffer.append(it->first + ": " + it->second + "\r\n");
	}
	buffer.append("\r\n");
	//End of headers + body
	if (this->body != "")
	{
		buffer.append(this->body);
	}
	return (buffer);
}
