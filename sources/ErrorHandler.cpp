#include "../include/webserv.hpp"

ErrorHandler::ErrorHandler()
{

}

ErrorHandler::ErrorHandler(const ErrorHandler &original)
{

}

ErrorHandler		&ErrorHandler::operator=(const ErrorHandler &original)
{

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
		std::string		path = config.root + config.error_page.at(status_code);
		std::ifstream	file(path.c_str());
		
		if (!file)
		{
			std::string		body;
			if (status_code == 400)
			{
				body = "<html><body><h1>400 Bad Request</h1><p>Your browser sent a request that this server could not understand.</p></body></html>";
			}
			else if (status_code == 403)
			{
				body = "<html><body><h1>403 Forbidden</h1><p>You don't have permission to access this resource.</p></body></html>";
			}
			else if (status_code == 404)
			{
				body = "<html><body><h1>404 Not Found</h1><p>The requested URL was not found on this server.</p></body></html>";
			}
			else if (status_code == 405)
			{
				body = "<html><body><h1>405 Method Not Allowed</h1><p>The method is not allowed for the requested URL.</p></body></html>"; 
			}
			else if (status_code == 413)
			{
				body = "<html><body><h1>413 Payload Too Large</h1><p>The uploaded data exceeds the maximum allowed size.</p></body></html>";
			}
			else if (status_code == 500)
			{
				body = "<html><body><h1>500 Internal Server Error</h1><p>Something went wrong on the server.</p></body></html>";
			}
			else if (status_code == 501)
			{
				body = "<html><body><h1>501 Not Implemented</h1><p>The requested method is not supported by the server.</p></body></html>";
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
			body = "<html><body><h1>400 Bad Request</h1><p>Your browser sent a request that this server could not understand.</p></body></html>";
		}
		else if (status_code == 403)
		{
			body = "<html><body><h1>403 Forbidden</h1><p>You don't have permission to access this resource.</p></body></html>";
		}
		else if (status_code == 404)
		{
			body = "<html><body><h1>404 Not Found</h1><p>The requested URL was not found on this server.</p></body></html>";
		}
		else if (status_code == 405)
		{
			body = "<html><body><h1>405 Method Not Allowed</h1><p>The method is not allowed for the requested URL.</p></body></html>"; 
		}
		else if (status_code == 413)
		{
			body = "<html><body><h1>413 Payload Too Large</h1><p>The uploaded data exceeds the maximum allowed size.</p></body></html>";
		}
		else if (status_code == 500)
		{
			body = "<html><body><h1>500 Internal Server Error</h1><p>Something went wrong on the server.</p></body></html>";
		}
		else if (status_code == 501)
		{
			body = "<html><body><h1>501 Not Implemented</h1><p>The requested method is not supported by the server.</p></body></html>";
		}
		response.headers["Content-Type"] = "text/html";
	}
	response.headers["Content-Length"] = std::to_string(response.body.size());

	return (response);
}
