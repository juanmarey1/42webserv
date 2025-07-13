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
	if (this->headers.count("Content-Type"))
	{
		buffer.append("Content-Type: " + this->headers.at("Content-Type") + "\r\n");
	}
	if (this->headers.count("Content-Length"))
	{
		buffer.append("Content-Length: " + this->headers.at("Content-Length") + "\r\n");
	}
	if (this->headers.count("Connection"))
	{
		buffer.append("Connection: " + this->headers.at("Connection") + "\r\n");
	}
	buffer.append("\r\n");
	//End of headers + body
	if (this->body != "")
	{
		buffer.append(this->body);
	}
	return (buffer);
}
