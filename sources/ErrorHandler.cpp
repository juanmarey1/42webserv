#include "../include/webserv.hpp"

ErrorHandler::ErrorHandler()
{

}

ErrorHandler::ErrorHandler(const ErrorHandler &original)
{
	*this = original;
}

ErrorHandler		&ErrorHandler::operator=(const ErrorHandler &original)
{
	if (this != &original)
	{

	}
	return (*this);

}

ErrorHandler::~ErrorHandler()
{

}


//Functions
HttpResponse	ErrorHandler::generateHttpResponse(int status_code, const ServerConfig &config)
{
	HttpResponse	response;

	//set stattus line
	response.status_code = status_code;

	//check if there is an error page configured for this error in this server
	if (config.error_page.count(status_code))
	{
		//ruta absoluta -> root + error page
		std::string rootError = config.root;
		std::string errorPagePath = config.error_page.at(status_code);
		if (Utils::ends_with(rootError,"/"))
		{
			rootError = rootError.substr(0, rootError.length() - 1);
		}
		if (errorPagePath[0] == '/')
		{
			errorPagePath = errorPagePath.substr(1);
		}
		std::string		path = rootError + "/" + errorPagePath;
		std::ifstream	file(path.c_str());
		
		if (!file)
		{
			std::string		body;
			if (status_code == 400)
			{
				response.body = "<html><body><h1>400 Bad Request</h1><p>Your browser sent a request that this server could not understand.</p></body></html>";
				response.status_text = "Bad Request";
			}
			else if (status_code == 403)
			{
				response.body = "<html><body><h1>403 Forbidden</h1><p>You don't have permission to access this resource.</p></body></html>";
				response.status_text = "Forbidden";
			}
			else if (status_code == 404)
			{
				response.body = "<html><body><h1>404 Not Found</h1><p>The requested URL was not found on this server.</p></body></html>";
				response.status_text = "Not Found";
			}
			else if (status_code == 405)
			{
				response.body = "<html><body><h1>405 Method Not Allowed</h1><p>The method is not allowed for the requested URL.</p></body></html>"; 
				response.status_text = "Method Not Allowed";
			}
			else if (status_code == 413)
			{
				response.body = "<html><body><h1>413 Payload Too Large</h1><p>The uploaded data exceeds the maximum allowed size.</p></body></html>";
				response.status_text = "Payload Too Large";
			}
			else if (status_code == 500)
			{
				response.body = "<html><body><h1>500 Internal Server Error</h1><p>Something went wrong on the server.</p></body></html>";
				response.status_text = "Internal Server Error";
			}
			else if (status_code == 501)
			{
				response.body = "<html><body><h1>501 Not Implemented</h1><p>The requested method is not supported by the server.</p></body></html>";
				response.status_text = "Not Implemented";
			}
			response.headers["Content-Type"] = "text/html";
		}
		else 
		{
			std::stringstream buffer;
			buffer << file.rdbuf();
			response.body = buffer.str();
			response.headers["Content-Type"] = "text/html";
		}
	}
	else
	{
		std::string		body;
		if (status_code == 400)
		{
			response.body = "<html><body><h1>400 Bad Request</h1><p>Your browser sent a request that this server could not understand.</p></body></html>";
			response.status_text = "Bad Request";
		}
		else if (status_code == 403)
		{
			response.body = "<html><body><h1>403 Forbidden</h1><p>You don't have permission to access this resource.</p></body></html>";
			response.status_text = "Forbidden";
		}
		else if (status_code == 404)
		{
			response.body = "<html><body><h1>404 Not Found</h1><p>The requested URL was not found on this server.</p></body></html>";
			response.status_text = "Not Found";
		}
		else if (status_code == 405)
		{
			response.body = "<html><body><h1>405 Method Not Allowed</h1><p>The method is not allowed for the requested URL.</p></body></html>"; 
			response.status_text = "Method Not Allowed";
		}
		else if (status_code == 413)
		{
			response.body = "<html><body><h1>413 Payload Too Large</h1><p>The uploaded data exceeds the maximum allowed size.</p></body></html>";
			response.status_text = "Payload Too Large";
		}
		else if (status_code == 500)
		{
			response.body = "<html><body><h1>500 Internal Server Error</h1><p>Something went wrong on the server.</p></body></html>";
			response.status_text = "Internal Server Error";
		}
		else if (status_code == 501)
		{
			response.body = "<html><body><h1>501 Not Implemented</h1><p>The requested method is not supported by the server.</p></body></html>";
			response.status_text = "Not Implemented";
		}
		response.headers["Content-Type"] = "text/html";
	}
	response.headers["Connection"] = "close";
	std::ostringstream oss;
	oss << response.body.size();
	response.headers["Content-Length"] = oss.str();

	return (response);
}
