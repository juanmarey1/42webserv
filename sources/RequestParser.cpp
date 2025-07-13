#include "../include/webserv.hpp"

RequestParser::RequestParser()
{
	this->phase = REQUEST_LINE;
}

RequestParser::RequestParser(const RequestParser &original)
{
	this->phase = original.phase;
	this->request = original.request;
}

RequestParser::~RequestParser()
{

}

RequestParser	&RequestParser::operator=(const RequestParser &original)
{
	if (this != &original)
	{
		this->phase = original.phase;
		this->request = original.request;
	}
	return (*this);
}


//Functions
bool			RequestParser::isRequestComplete() const
{
	if (this->phase == COMPLETE)
	{
		return (true);
	}
	else 
	{
		return (false);
	}
}


HttpRequest		RequestParser::getHttpRequest() const 
{
	return (this->request);
}


void			RequestParser::parseLine(const std::string &line)
{
	//first parser the request line
	if (this->phase == REQUEST_LINE)
	{
		std::vector<std::string>	firstLine;
		
		firstLine = Utils::splitBySpaces(line);
		if (firstLine.size() != 3)
		{
			throw InvalidRequestLineExcept();
		}
		//Checks the method
		if (firstLine[0] != "GET" || firstLine[0] != "POST" || firstLine[0] != "DELETE")
		{
			throw InvalidRequestLineExcept();
		}
		this->request.method = firstLine[0];
		this->request.uri = firstLine[1];
		//Checks the version
		if (firstLine[2] != "HTTP/1.1")
		{
			throw InvalidRequestLineExcept();
		}
		this->request.version = firstLine[2];
		this->phase = HEADERS;
	}
	else if (this->phase == HEADERS)
	{
		//Checks the end of the headers
		if (line == "\r\n")
		{
			if (this->request.headers.count("Content-Length") || this->request.headers.count("Content-Type"))
			{
				if ((this->request.headers.count("Content-Length") && !(this->request.headers.count("Content-Type"))) || (!(this->request.headers.count("Content-Length"))) && (this->request.headers.count("Content-Type")))
				{
					throw InvalidHeadersExcept();
				}
				this->phase = BODY;
			}
			else
			{
				this->phase = COMPLETE;
			}
			return ;
		}
		//Gets the line and gets the header and the value
		std::vector<std::string>	headerLine;

		headerLine = Utils::splitBySpaces(line);
		std::string	header = headerLine[0];
		std::string	value = headerLine[1];
		header = header.substr(0, header.size());
		//Checks the header
		if (header == "Host")
		{
			this->request.headers[header] = value;
		}
		else if (header == "User-Agent")
		{
			this->request.headers[header] = value;
		}
		else if (header == "Accept")
		{
			this->request.headers[header] = value;
		}
		else if (header == "Content-Length")
		{
			this->request.headers[header] = value;
		}
		else if (header == "Content-Type")
		{
			this->request.headers[header] = value;
		}
		else if (header == "Connection")
		{
			this->request.headers[header] = value;
		}
		else if (header == "Accept-Encoding")
		{
			this->request.headers[header] = value;
		}
	}
	else if (this->phase == BODY)
	{
		this->request.body.append(line);
	}
	else if (this->phase == COMPLETE)
	{
		return;
	}
}
