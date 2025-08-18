#include "../include/webserv.hpp"

ConnectionHandler::ConnectionHandler()
{
	this->fd = -1;
	this->buffer = "";
	this->state = READING;
	this->lastActivity = std::time(NULL);
	this->sendBuffer = "";
	this->sendOffset = 0;
}

ConnectionHandler::ConnectionHandler(int fd)
{
	this->fd = fd;
	this->buffer = "";
	this->state = READING;
	this->lastActivity = std::time(NULL);
	this->sendBuffer = "";
	this->sendOffset = 0;
}

ConnectionHandler::ConnectionHandler(const ConnectionHandler &original)
{
	this->fd = original.fd;
	this->buffer = original.buffer;
	this->state = original.state;
	this->lastActivity = original.lastActivity;
	this->sendBuffer = original.sendBuffer;
	this->sendOffset = original.sendOffset;
	this->request = original.request;
	this->response = original.response;
	this->config = original.config;
}

ConnectionHandler	&ConnectionHandler::operator=(const ConnectionHandler &original)
{
	if (this != &original)
	{
		this->fd = original.fd;
		this->buffer = original.buffer;
		this->state = original.state;
		this->lastActivity = original.lastActivity;
		this->sendBuffer = original.sendBuffer;
		this->sendOffset = original.sendOffset;
		this->request = original.request;
		this->response = original.response;
		this->config = original.config;
	}
	return (*this);
}

ConnectionHandler::~ConnectionHandler()
{
	if (this->fd)
	{
		close(this->fd);
		this->fd = -1;
	}
}

//Functions
bool				ConnectionHandler::writeResponse(const ServerConfig &server)
{
	if (this->state != WRITING)
	{
		return (false);
	}
	//We build the object HttpResponse with the request Object and the server and locations.
	ResponseBuilder		builder;

	// std::cout << "[TRACE] - " << this->request.method << " " << this->request.uri << " " << this->request.version << std::endl;
	if (this->request.error <= 0)
	{
		if (int(this->request.body.size()) > this->config.second.client_max_body_size)
		{
			this->response = ErrorHandler::generateHttpResponse(413, server);
		}
		else 
		{
			this->response = builder.generateHttpResponse(this->request, this->config);
		}
	}
	else 
	{
		this->response = ErrorHandler::generateHttpResponse(this->request.error, server);
	}
	//We have now the object and we create the text response
	this->sendBuffer = this->response.build();

	//Now we send() the bytes left of the request for sending and we start since the last byte we sent last time (if it is the ffirst time we are sending, we start in 0)
	ssize_t sent = send(this->fd, this->sendBuffer.c_str() + this->sendOffset, sendBuffer.size() - sendOffset, 0);
	//If sent <= 0, an error occured and we have to close connection
	if (sent <= 0)
	{
		if (sent < 0)
		{
    		if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) 
			{
    		    // Temporary failure, caller can try again later
    		    return true;
    		}
    	return false; // real error, close connection
		}
	}
	this->sendOffset = this->sendOffset + sent;
	if (this->sendOffset == this->sendBuffer.size())
	{
		this->state = READY;
	}
	return (true);
}


bool				ConnectionHandler::isReadyToWrite()
{
	if (this->state == WRITING)
	{
		return (true);
	}
	else 
	{
		return (false);
	}
}


bool				ConnectionHandler::isRequestComplete()
{
	if (this->state != READING)
	{
		return (true);
	}
	else 
	{
		return (false);
	}
}


bool				ConnectionHandler::isResponseComplete()
{
	if (this->state == READY)
	{
		return (true);
	}
	else 
	{
		return (false);
	}
}


void 				ConnectionHandler::cleanConnection()
{
	this->state = READING;
	this->buffer = "";
	this->sendBuffer = "";
	this->sendOffset = 0;
	this->request.body = "";
	this->request.headers.clear();
	this->request.error = -1;
	this->request.method = "";
	this->request.uri = "";
	this->request.version = "";
	this->response.body = "";
	this->response.headers.clear();
	this->response.status_code = -1;
	this->response.status_text = "";
	return;
}


bool				ConnectionHandler::parseRequest()
{
	//We search for the end of headers, if we could not find it yet, we need to keep receiving information
	size_t endOfHeaders = this->buffer.find("\r\n\r\n");

	if (endOfHeaders == std::string::npos)
	{
		return (true);
	}

	//When we have the complete headers, we parse the request (or at least what we have of them)
	RequestParser		parser;
	std::string			line;
	size_t				pos = 0;

	while (Utils::getNextLine(buffer, pos, line))
	{
		parser.parseLine(line);
	}

	//If our request is complete, we save our HttpRequest and we will use it to write our response. We change our connection state to WRITING (the response)
	if (parser.isRequestComplete())
	{
		this->state = WRITING;
		this->request = parser.getHttpRequest();
		std::cout << "[TRACE] - " << this->request.method << " " << this->request.uri << " " << this->request.version << std::endl;
		return (true);
	}
	
	//If our request is still  not complete, we will just wait for more text and parse it all over again with the new text to see if it is now complete
	else 
	{
		return (true);
	}
}


bool				ConnectionHandler::readRequest()
{
	while (true)
	{
		if (state != READING)
		{
			return (true);
		}
		//temp buffer to recv datta
		char		tmpBuffer[BUFFER_SIZE];
		//Read raw bytes 
		ssize_t bytes = recv(this->fd, tmpBuffer, sizeof(tmpBuffer), 0);

		if (bytes <= 0)
		{
			if (bytes < 0)
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK)
				{
					return (parseRequest());
				}
			}
			//An error ocurred
			return (false);
		}

		//Append the data to your internal request buffer
		this->buffer.append(tmpBuffer, bytes);
		//Update last activity timestamp
		this->lastActivity = std::time(NULL);
		// std::cout << "Last activity : " << this->lastActivity << std::endl;
		//Try to parse full HTTP request, returns true if we there is no error.
	}
	return (parseRequest());
}
