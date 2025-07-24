#include "../include/webserv.hpp"

ResponseBuilder::ResponseBuilder()
{

}

ResponseBuilder::ResponseBuilder(const ResponseBuilder &original)
{

	*this = original;
}

ResponseBuilder		&ResponseBuilder::operator=(const ResponseBuilder &original)
{
	if (this != &original)
	{

	}
	return (*this);

}

ResponseBuilder::~ResponseBuilder()
{

}


//Functions
std::string			ResponseBuilder::getMimeType(const std::string &path)
{
	if (Utils::ends_with(path, ".html"))
	{
		return ("text/html");
	}
	if (Utils::ends_with(path, ".css"))
	{
		return ("text/css");
	}
	if (Utils::ends_with(path, ".js"))
	{
		return ("application/javascript");
	}
	if (Utils::ends_with(path, ".png"))
	{
		return ("image/png");
	}
	if (Utils::ends_with(path, ".jpg") || Utils::ends_with(path, ".jpeg"))
	{
		return ("image/jpeg");
	}
	if (Utils::ends_with(path, ".txt"))
	{
		return ("text/plain");
	}
	return ("application/octet-stream");
}


std::string			ResponseBuilder::getExtensionType(const std::string &mime)
{
	if (mime == "text/html")
	{
		return (".html");
	}
	if (mime == "text/css")
	{
		return (".css");
	}
	if (mime == "application/javascript")
	{
		return (".js");
	}
	if (mime == "image/png")
	{
		return (".png");
	}
	if (mime == "image/jpeg")
	{
		return (".jpeg");
	}
	if (mime == "text/plain")
	{
		return (".txt");
	}
	return ("");
}


std::string			ResponseBuilder::getFileContent(const std::string &path)
{
	std::ifstream	file(path.c_str());
	std::string		buffer;
	std::string		line;

	if (!file.is_open())
	{
		throw InvalidPathExcept();
	}
	while (std::getline(file, line))
	{
		buffer.append(line);
	}
	file.close();
	return(buffer);
}


std::string 		ResponseBuilder::generateAutoIndexHtml(const std::string& dirPath)
{
    std::string html = "<html><head><title>Index of " + dirPath + "</title></head><body>";
    html += "<h1>Index of " + dirPath + "</h1><ul>";

    DIR* dir = opendir(dirPath.c_str());
    if (!dir)
        return ("");

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        std::string name = entry->d_name;
        if (name == ".") continue;

        std::string href = name;
        if (entry->d_type == DT_DIR)
            href += "/";

        html += "<li><a href=\"" + href + "\">" + name + "</a></li>";
    }
    closedir(dir);

    html += "</ul></body></html>";
    return html;
}


std::string			ResponseBuilder::getPath(const bool exactMatch, const std::string method, const std::string serverRoot, const std::string root, const std::string locPrefix, const std::string uri, const std::vector<std::string> index, const bool autoindex)
{
	std::string		path;
	std::string		uriMinusLocPrefix;

	//removed matched location prefix from uri
	if (exactMatch)
	{
		std::string newRoot = root;
		if (root[root.size() - 1] == '/')
		{
			newRoot = root.substr(0, root.size() - 1);
		}
		std::string newUri = uri;
		if (uri[0] != '/')
		{
			newUri = "/" + uri;
		}
		std::cout << "root: " << newRoot << " + uri: " << newUri << " = path: " << path << std::endl;
		path = newRoot + newUri;
	}
	else 
	{
		if (serverRoot == root)
		{
			std::string newRoot = root;
			if (root[root.size() - 1] == '/')
			{
				newRoot = root.substr(0, root.size() - 1);
			}
			std::string newUri = uri;
			if (uri[0] != '/')
			{
				newUri = "/" + uri;
			}
			std::cout << "server root: " << newRoot << " + uri: " << newUri << " = path: " << path << std::endl;
			path = newRoot + newUri;
		}
		else 
		{
			std::string newRoot = root;
			if (root[root.size() - 1] == '/')
			{
				newRoot = root.substr(0, root.size() - 1);
			}
			std::string	newUri = uri.substr(locPrefix.size());
			if (newUri[0] != '/')
			{
				newUri = "/" + newUri;
			}
			std::cout << "location root: " << newRoot << " + uri - location path: " << newUri << " = path: " << path << std::endl;
			path = newRoot + newUri;
		}
	}

	std::cout << path << std::endl;
	
	//If the path is a directory, we match it with the indexes provided and we create the path
	if (Utils::isDirectory(path))
	{
		size_t i;
		if (!index.empty())
		{
			for (i = 0; i < index.size(); i++)
			{
				std::string filePath = path + "/" + index[i];
				if (Utils::fileExists(filePath))
				{
					return (filePath);
				}
			}
			//Later do the autoindex part
			if (autoindex)
			{
				std::string autoIn = generateAutoIndexHtml(path);
				if (autoIn == "")
				{
					return ("403");
				}
				return (autoIn);
			}
			else 
			{
				return ("403");
			}
		}
		else 
		{
			if (autoindex)
			{
				std::string autoIn = generateAutoIndexHtml(path);
				if (autoIn == "")
				{
					return ("403");
				}
				return (autoIn);
			}
			else
			{
				return ("403");
			}
		}
	}
	if (Utils::fileExists(path))
	{
		if (method == "GET")
		{
			if (access(path.c_str(), R_OK) != 0)
			{
				return ("403");
			}
		}
		else if (method == "POST")
		{
			if (access(path.c_str(), W_OK) != 0 || access(path.c_str(), F_OK) != 0)
			{
				return ("403");
			}
		}
		else if (method == "DELETE")
		{
			if (access(path.c_str(), W_OK) != 0)
			{
				return ("403");
			}
		}
		return (path);
	}
	else 
	{
		return ("404");
	}
}


std::string			ResponseBuilder::generateFilename(const std::string &mime) {
    std::time_t t = std::time(NULL);
    std::tm* tm = std::localtime(&t);
	std::string		extension = getExtensionType(mime);
    
	//upload_ + date + extension
    std::ostringstream oss;
    char buffer[64];
	std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", tm);
	oss << "upload_" << buffer << extension;
    return oss.str();
}


HttpResponse		ResponseBuilder::generateHttpResponse(const HttpRequest &request, const std::pair<ServerConfig, LocationConfig>	 &config)
{
	HttpResponse	response;
	ServerConfig	server = config.first;
	LocationConfig	location = config.second;

	//1. We check if the method is allowed. If not -> error 405
	size_t i;
	for (i = 0; i < location.allowed_methods.size(); i++)
	{
		if (location.allowed_methods[i] == request.method)
		{
			break;
		}
	}
	if (i >= location.allowed_methods.size())
	{
		return (ErrorHandler::generateHttpResponse(405, server));
	}

	//2. method GET
	if (request.method == "GET")
	{
		if (location.cgi_extension != "" || location.cgi_path != "")
		{
			CGIHandler	cgiHandler;
			response = cgiHandler.executeScript(request, location, server);
		}
		else 
		{
			//First we get the path of the file.
			std::string		path = getPath(location.exactMatch, request.method, server.root, location.root, location.locationPath, request.uri, location.index, location.autoindex);
			if (path == "404")
			{
				return (ErrorHandler::generateHttpResponse(404, server));
			}
			else if (path == "403")
			{
				return (ErrorHandler::generateHttpResponse(403, server));
			}
			else if (path.substr(0, 6) == "<html>")
			{
				response.status_code = 200;
				response.status_text = "OK";
				response.headers["Content-Type"] = "text/html";
				std::stringstream ss1;
				ss1 << path.size();
				response.headers["Content-Length"] = ss1.str();
				if (request.headers.count("Connection"))
				{
					if (request.headers.find("Connection")->second == "close")
					{
						response.headers["Connection"] = "close";
					}
					else 
					{
						response.headers["Connection"] = "keep-alive";
					}
				}
				else 
				{
					response.headers["Connection"] = "keep-alive";
				}
				response.body = path;
				return (response);
			}
			//Once we obtain the path of the file: we open it and we get all the content
			std::string		fileContent = getFileContent(path);
			//We setup status code and text
			response.status_code = 200;
			response.status_text = "OK";
			//We declare Content-Length header
			std::stringstream ss;
			ss << fileContent.size();
			response.headers["Content-Length"] = ss.str();
			//We declare Content-Type (MIME-type)
			response.headers["Content-Type"] = getMimeType(path);
			//We check Connection header from request
			if (request.headers.count("Connection"))
			{
				if (request.headers.find("Connection")->second == "close")
				{
					response.headers["Connection"] = "close";
				}
				else 
				{
					response.headers["Connection"] = "keep-alive";
				}
			}
			else 
			{
				response.headers["Connection"] = "keep-alive";
			}
			//Now we save the fileContent as the body and we return the request
			response.body = fileContent;
		}
		return (response);
	}

	//3. method POST
	else if (request.method == "POST")
	{
		if (location.cgi_extension != "" || location.cgi_path != "")
		{
			CGIHandler	cgiHandler;
			response = cgiHandler.executeScript(request, location, server);
		}
		else 
		{
			//we search the path to upload and creeatte the name of the nuew file
			std::string uploadPath;

			if (location.upload_store != "")
			{
				uploadPath = location.upload_store;
			}
			else 
			{
				uploadPath = "/var/www/uploads";
			}
			std::string mime = request.headers.find("Content-Type")->second;
			std::string fileName = generateFilename(mime);
			uploadPath = uploadPath + "/" + fileName;

			//now we create the file and write all teh request body in it
			std::ofstream	file(uploadPath.c_str(), std::ios::binary);
			if (!file.is_open())
			{
				throw InvalidPathExcept();
			}
			file.write(request.body.c_str(), request.body.size());
			file.close();

			//now we create the body and check the connection header
			response.status_code = 201;
			response.status_text = "OK";
			if (request.headers.count("Connection"))
			{
				if (request.headers.find("Connection")->second == "close")
				{
					response.headers["Connection"] = "close";
				}
				else 
				{
					response.headers["Connection"] = "keep-alive";
				}
			}
			else 
			{
				response.headers["Connection"] = "keep-alive";
			}
			return (response);
		}
	}
	
	//4. method DELETE
	else 
	{
		//we get the file to remove
		std::string path = getPath(location.exactMatch, request.method, server.root, location.root, location.locationPath, request.uri, location.index, location.autoindex);
		//remove it
		if (std::remove(path.c_str()) == 0)
		{
			//We setup status code (we don't want a message body)
			response.status_code = 204;
			response.status_text = "OK";
			//We set up connection header
			if (request.headers.count("Connection"))
			{
				if (request.headers.at("Connection") == "close")
				{
					response.headers["Connection"] = "close";
				}
				else 
				{
					response.headers["Connection"] = "keep-alive";
				}
			}
			else 
			{
				response.headers["Connection"] = "keep-alive";
			}

			return (response);
		}
		else 
		{
			return (response);
			//Throw an error
		}
	}
	return (response);

}

