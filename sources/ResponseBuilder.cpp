#include "../include/webserv.hpp"

ResponseBuilder::ResponseBuilder()
{

}

ResponseBuilder::ResponseBuilder(const ResponseBuilder &original)
{

}

ResponseBuilder		&ResponseBuilder::operator=(const ResponseBuilder &original)
{

}

ResponseBuilder::~ResponseBuilder()
{

}


//Functions
HttpResponse		ResponseBuilder::generateHttpResponse(const HttpRequest &request, const std::pair<ServerConfig, LocationConfig>	 &config)
{
	HttpResponse	response;
	ServerConfig	server = config.first;
	LocationConfig	location = config.second;
	std::string		method = request.method;

	//1. We check if the method is allowed
	methods = location.allowed_methods;
	for (int i = 0; i < methods.size(); i++)
	{
		if (methods[i] == request.method)
		{
			break;
		}
	}

}

