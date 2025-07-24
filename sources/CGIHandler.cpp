#include "../include/webserv.hpp"

CGIHandler::CGIHandler()
{
	this->cgiOutput = "";
}

CGIHandler::CGIHandler(const CGIHandler &original)
{
	this->env = original.env;
	this->cgiOutput = original.cgiOutput;
}

CGIHandler		&CGIHandler::operator=(const CGIHandler &original)
{
	if (this != &original)
	{
		this->env = original.env;
		this->cgiOutput = original.cgiOutput;
	}
	return (*this);
}

CGIHandler::~CGIHandler()
{

}


//Functions
void				CGIHandler::setupEnv(const HttpRequest &request, const LocationConfig &location)
{
	this->env.clear();
	this->env.push_back("REQUEST_METHOD=" + request.method);
	std::string filename = location.root + request.uri;
	size_t		questionQuery = filename.find("?");
	if (questionQuery != std::string::npos)
	{
		this->env.push_back("SCRIPT_NAME=" + filename.substr(0, questionQuery));
		filename = location.cgi_path + request.uri.substr(location.locationPath.size());
		questionQuery = filename.find("?");
		this->env.push_back("SCRIPT_FILENAME=" + filename.substr(0, questionQuery));
	}
	else 
	{
		this->env.push_back("SCRIPT_NAME=" + filename);
		this->env.push_back("SCRIPT_FILENAME=" + location.cgi_path + request.uri.substr(location.locationPath.size()));
	}
	std::string	filenameEnv = "SCRIPT_FILENAME=";
	if (!Utils::fileExists(this->env[2].substr(filenameEnv.length())))
	{
		return ;
	}
	
	if (questionQuery == std::string::npos)
	{
		this->env.push_back("QUERY_STRING=");
	}
	else 
	{
		this->env.push_back("QUERY_STRING=" + filename.substr(questionQuery + 1));
	}

	if (request.method == "POST")
	{
		std::ostringstream oss;
		oss << request.body.size();
		this->env.push_back("CONTENT_LENGTH=" + oss.str());
	}
	else
	{
		this->env.push_back("CONTENT_LENGTH=");
	}
	std::map<std::string, std::string>::const_iterator it = request.headers.find("Content-Type");
	if (it != request.headers.end()) {
	    this->env.push_back("CONTENT_TYPE=" + it->second);
	} else {
	    this->env.push_back("CONTENT_TYPE="); // or some default value
	}
	
	this->env.push_back("SERVER_PROTOCOL=HTTP/1.1");

	std::map<std::string, std::string>::const_iterator it2 = request.headers.find("Host");
	if (it2 != request.headers.end()) {
	    this->env.push_back("HTTP_HOST=" + it2->second);
	} else {
	    return ;
	}

	std::map<std::string, std::string>::const_iterator it3 = request.headers.find("User-Agent");
	if (it3 != request.headers.end()) {
	    this->env.push_back("HTTP_USER_AGENT=" + it3->second);
	} else {
	    this->env.push_back("HTTP_USER_AGENT="); // or some default value
	}

	//DEBUGGING
	for (size_t i = 0; i < this->env.size(); i++)
	{
		std::cout << this->env[i] << std::endl;
	}

}


int				CGIHandler::forkExecute(const std::string &scriptPath, const HttpRequest &request)
{
	int	pipeToChild[2]; //parent writes to child standard input
	int	pipeFromChild[2]; //parent reads from child stdout
	
	pipe(pipeToChild);
	pipe(pipeFromChild);

	//debugging
	// std::vector<char*> envp;
    // for (size_t i = 0; i < this->env.size(); i++) {
	// 	envp.push_back(const_cast<char*>(this->env[i].c_str()));
	// }
	// envp.push_back(NULL);
	// for (size_t i = 0; i < this->env.size(); i++)
	// {
	// 	std::cout << envp[i] << std::endl;
	// }
	pid_t	pid =  fork();

	//In the child::
	if (pid == 0)
	{
		//we first close the ends of the pipe we are not using
		close(pipeToChild[1]);
		close(pipeFromChild[0]);

		//stdin redirected to read end of pipeToChild
		dup2(pipeToChild[0], STDIN_FILENO);
		close(pipeToChild[0]);

		//stdout redirected to write end of pipeFromChild
		dup2(pipeFromChild[1], STDOUT_FILENO);
		close(pipeFromChild[1]);

		//prepare args
		char *argv[] = {const_cast<char*>(scriptPath.c_str()), NULL};

		//prepare env
		std::vector<char*> envp;
    	for (size_t i = 0; i < this->env.size(); i++) {
			envp.push_back(const_cast<char*>(this->env[i].c_str()));
		}
		envp.push_back(NULL);

		execve(scriptPath.c_str(), argv, envp.data());

		exit(1);

	}
	else 
	{
		//Close unused ends
		close(pipeToChild[0]);
		close(pipeFromChild[1]);

		//Write request body to childs stdin and close it at the end
		write(pipeToChild[1], request.body.c_str(), request.body.size());
		close(pipeToChild[1]);
		
		//READ CGI OUTPUT
		char		buffer[BUFFER_SIZE];
		ssize_t		n;
		//Store it everything in cgiOutput
		while ((n = read(pipeFromChild[0], buffer, sizeof(buffer))) > 0)
		{
			this->cgiOutput.append(buffer, n);
		}
		//DEBUGGING
		std::cout << cgiOutput << std::endl;

		close(pipeFromChild[0]);

		//Wait for child to finish
		int	status;
		waitpid(pid, &status, 0);
		//Debugging:
		// std::cout << status << std::endl;
		if (status != 0)
		{
			return(-1);
		}
		return (0);

	}
	//By the end we have the entire CGI response
}


HttpResponse		CGIHandler::executeScript(const HttpRequest &request, const LocationConfig &location, const ServerConfig &server)
{
	HttpResponse	response;

	//1. We store the env
	setupEnv(request, location);

	//debugging: 
	// std::cout << this->env[2] << std::endl;
	// std::cout << std::endl;
	std::string	filenameEnv = "SCRIPT_FILENAME=";
	//If file does not exist return 404
	if (!Utils::fileExists(this->env[2].substr(filenameEnv.length())))
	{
		return (ErrorHandler::generateHttpResponse(404, server));
	}
	//If no access to file return 403
	if (access(this->env[2].substr(filenameEnv.length()).c_str(), X_OK) < 0)
	{
		return (ErrorHandler::generateHttpResponse(403, server));
	}

	//Si no hay host en la request, return bad request 400
	std::map<std::string, std::string>::const_iterator it2 = request.headers.find("Host");
	if (it2 == request.headers.end()) {
	    return (ErrorHandler::generateHttpResponse(400, server));
	}


	//2. forkandexecute. 
	std::string scriptFilename = this->env[2].substr(strlen("SCRIPT_FILENAME="));
	//if execve ffails, return 500
	if (forkExecute(scriptFilename, request) < 0)
	{
		return (ErrorHandler::generateHttpResponse(500, server));
	}

	//3. We have now executed and have the entire CGI output, we now parse it
	std::string			buffer = this->cgiOutput;
	std::string			line;
	size_t				pos = 0;

	response.status_code = 200;
	response.status_text = "OK";
	if (buffer.find("\n\n") == std::string::npos && !buffer.empty())
	{
		return (ErrorHandler::generateHttpResponse(500, server));
	}
	size_t		pos1 = buffer.find("\n\n") + 2;
	if (pos1 >= buffer.size() && request.method != "GET")
	{
		return (ErrorHandler::generateHttpResponse(500, server));
	}
	int k = 0;
	while (Utils::getNextLine(buffer, pos, line) && line != "\r\n" && line != "\n" && !line.empty())
	{
		k++;
		std::vector<std::string>	headerLine;
		
		headerLine = Utils::splitBySpaces(line);
		//parse headers
		std::string	header = headerLine[0];
		std::string	value = headerLine[1];
		if (header.find(":") == std::string::npos)
		{
			return (ErrorHandler::generateHttpResponse(500, server));
		}
		header = header.substr(0, header.size() - 1);
		
		if (header == "Status")
		{
			if (k != 1)
			{
				return (ErrorHandler::generateHttpResponse(500, server));
			}
			if (headerLine.size() != 3)
			{
				return (ErrorHandler::generateHttpResponse(500, server));
			}
			response.status_code = std::atoi(headerLine[1].c_str());
			response.status_text = headerLine[2];
		}
		if (header == "Content-Type")
		{
			response.headers[header] = value;
		}
		else if (header == "Location")
		{
			response.headers[header] = value;
		}
		else if (header == "Content-Length")
		{
			response.headers[header] = value;
		}
	}
	if (request.headers.find("Connection") == request.headers.end() || request.headers.find("Connection")->second == "keep-alive")
	{
		response.headers["Connection"] = "keep-alive";
	}
	else if (request.headers.find("Connection")->second == "close")
	{
		response.headers["Connection"] = "close";
	}
	else 
	{
		return (ErrorHandler::generateHttpResponse(500, server));
	}
	if (response.headers.find("Location") != response.headers.end())
	{
		response.status_code = 302;
		response.status_text = "Found";
	}
	if (response.headers.find("Content-Type") == response.headers.end() && this->cgiOutput.size() != 0 && request.method != "GET" && response.headers.find("Location") == response.headers.end())
	{
		return (ErrorHandler::generateHttpResponse(500, server));
	}
	else if (response.headers.find("Content-Type") == response.headers.end() && this->cgiOutput.size() == 0)
	{
		//If cgiOutput is empty, return as normal.
		response.headers["Content-Type"] = "text/plain";
	}
	//parse body
	while (Utils::getNextLine(buffer, pos, line))
	{
		response.body.append(line);
		response.body.append("\n");
	}
	//Borra la ultima linea del final
	if (!response.body.empty())
	{
    	response.body.erase(response.body.size() - 1);
	}
	std::ostringstream oss;
	oss << response.body.size(); 
	response.headers["Content-Length"] = oss.str();

	return (response);
}
