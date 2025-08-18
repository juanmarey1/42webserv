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
			// std::cout << "Invalid first lline" << std::endl;
			this->phase = COMPLETE;
			this->request.error = 400;
			return ;
		}
		//Checks the method
		if (firstLine[0] != "GET" && firstLine[0] != "POST" && firstLine[0] != "DELETE")
		{
			this->phase = COMPLETE;
			this->request.error = 405;
			return ;
		}
		this->request.method = firstLine[0];
		this->request.uri = firstLine[1];
		if (this->request.uri[0] != '/')
		{
			// std::cout << "Invalid uri" << std::endl;
			this->phase = COMPLETE;
			this->request.error = 400;
			return ;
		}
		if (this->request.uri.size() > 1 && this->request.uri[1] == '.' && this->request.uri[2] == '.')
		{
			// std::cout << "Invalid uri" << std::endl;
			this->phase = COMPLETE;
			this->request.error = 400;
			return ;
		}
		//Checks the version
		if (firstLine[2] != "HTTP/1.1\r")
		{
			// std::cout << "Invalid HTTP version" << std::endl;
			this->phase = COMPLETE;
			this->request.error = 400;
			return ;
		}
		this->request.version = firstLine[2];
		this->phase = HEADERS;
	}
	else if (this->phase == HEADERS)
	{
		//Checks the end of the headers
		if (line == "\r")
		{
			//We check if it lacks the Host header (required in HTTP 1.1)
			if (!this->request.headers.count("Host"))
			{
				// std::cout << "Missing host" << std::endl;
				this->phase = COMPLETE;
				this->request.error = 400;
				return;
			}
			if (this->request.headers.find("Host")->second.find(":"))
			{
				this->request.headers["Host"] = this->request.headers["Host"].substr(0, this->request.headers.find("Host")->second.find(":") );
			}
			if (this->request.method == "POST" && !this->request.headers.count("Content-Length"))
			{
				// std::cout << "Missing content length" << std::endl;
				this->phase = COMPLETE;
				this->request.error = 400;
				return;
			}
			if (!this->request.headers.count("Connection"))
			{
				this->request.headers["Connection"] = "keep-alive";
			}
			//We check if it should be a body or not
			if (this->request.headers.count("Content-Length") || this->request.headers.count("Content-Type"))
			{
				//If Content-Length and Content-Type are in the headers -> BODY expected
				if ((this->request.headers.count("Content-Length") && !(this->request.headers.count("Content-Type"))) || ((!(this->request.headers.count("Content-Length"))) && (this->request.headers.count("Content-Type"))))
				{
					this->phase = COMPLETE;
					this->request.error = 400;
					return;
				}
				for (size_t j = 0; j < this->request.headers["Content-Length"].size(); j++)
				{
					if (!std::isdigit(this->request.headers["Content-Length"][j]))
					{
						this->phase = COMPLETE;
						this->request.error = 400;
						return;
					}
					else 
					{
						j++;
					}
				}
				if (!this->request.body.empty())
				{
					this->request.body.clear();
				}
				if (std::atoi(this->request.headers.find("Content-Length")->second.c_str()) == 0)
				{
					this->phase = COMPLETE;
				}
				else 
				{
					this->phase = BODY;

				}
			}
			else
			{
				//Else, no body is expected -> COMPLETE
				this->phase = COMPLETE;
			}
			return ;
		}
		//Gets the line and gets the header and the value
		std::vector<std::string>	headerLine;

		std::string lineCpy = line.substr(0, line.size() - 1);
		headerLine = Utils::splitBySpaces(lineCpy);
		std::string header;
		std::string value;
		if (lineCpy.find(":") == std::string::npos)
		{
			this->phase = COMPLETE;
			this->request.error = 400;
			return ;
		}
		if (lineCpy.find(":") > lineCpy.find(" ") && headerLine.size() != 3 && lineCpy.find(" ") != 0)
		{
			this->phase = COMPLETE;
			this->request.error = 400;
			return ;
		}
		if (headerLine.size() == 3 && headerLine[1] == ":")
		{
			header = headerLine[0];
			value = headerLine[2];
		}
		else if (headerLine.size() != 1)
		{
			value = headerLine[1];
			if (headerLine[0][headerLine[0].size() - 1] == ':')
			{
				header = headerLine[0].substr(0, headerLine[0].size() - 1);
			}
			else 
			{
				header = headerLine[0];
				if (value[0] == ':')
				{
					value = value.substr(1);
				}
			}
		}
		else 
		{
			if (headerLine[0][0] == ':')
			{
				this->phase = COMPLETE;
				this->request.error = 400;
				return ;
			}
			header = headerLine[0].substr(0, headerLine[0].find(':'));
			value = headerLine[0].substr(headerLine[0].find(':') + 1);
		}
		//Checks the header and store its value
		if (Utils::toLowerAlphaOnly(header) == "host")
		{
			this->request.headers["Host"] = value;
		}
		else if (Utils::toLowerAlphaOnly(header) == "user-agent")
		{
			this->request.headers["User-Agent"] = value;
		}
		else if (Utils::toLowerAlphaOnly(header) == "accept")
		{
			this->request.headers["Accept"] = value;
		}
		else if (Utils::toLowerAlphaOnly(header) == "content-length")
		{
			this->request.headers["Content-Length"] = value;
		}
		else if (Utils::toLowerAlphaOnly(header) == "content-type")
		{
			this->request.headers["Content-Type"] = value;
		}
		else if (Utils::toLowerAlphaOnly(header) == "connection")
		{
			this->request.headers["Connection"] = value;
			if (value != "keep-alive" && value != "close")
			{
				this->phase = COMPLETE;
				this->request.error = 400;
				return ;
			}
		}
		else if (Utils::toLowerAlphaOnly(header) == "accept-encoding")
		{
			this->request.headers["Accept-Encoding"] = value;
		}
	
	}
	else if (this->phase == BODY)
	{
		//Appends the new line to the body and sees if it has reached the size of the Content-Length header value.
		int 	contentLength = std::atoi(this->request.headers["Content-Length"].c_str());
		int		currentBytes;

		this->request.body.append(line);
		currentBytes = this->request.body.size();
		if (currentBytes >= contentLength)
		{
			//If it has reached the contentLength value, it substracts characters out of the size and declares the request COMPLETE
			this->request.body = this->request.body.substr(0, contentLength);
			this->phase = COMPLETE;
			return ;
		}
		else 
		{
			return ;
		}
	}
	else if (this->phase == COMPLETE)
	{
		return;
	}
}
