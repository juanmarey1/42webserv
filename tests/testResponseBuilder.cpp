// #include "../include/webserv.hpp"

// int main(void)
// {
// 	HttpRequest	request;
// 	request.method = "GET";
// 	request.uri = "/servDir/var/www/html/index.html";
// 	request.headers["Host"] = "fdsa";
// 	request.body = "Hello World";
// 	request.headers["Content-Type"] = "text/plain";
// 	request.headers["Content-Length"] = "11";
// 	request.headers["Connection"] = "close";
// 	Router		router;
// 	std::vector<ServerConfig>	servers;
// 	ConfigFileParser	configParser;
// 	servers	= configParser.parse("/home/jrey-roj/juanma/webserv/tests/var/www/routers.config");

// 	std::pair<ServerConfig, LocationConfig>	pairs = router.route(request, servers);
// 	for (size_t i = 0; i < servers.size(); i++)
// 	{
// 		if (servers[i] == pairs.first)
// 		{
// 			std::cout << "Server: " << i + 1 << std::endl; 
// 		}
// 	}
// 	for (size_t i = 0; i < pairs.first.locations.size(); i++)
// 	{
// 		if (pairs.first.locations[i] == pairs.second)
// 		{
// 			std::cout << "Location: " << i + 1 << std::endl; 
// 		}
// 	}
// 	ResponseBuilder	builder;
// 	HttpResponse		response = builder.generateHttpResponse(request, pairs);
// 	std::cout << response.build() << std::endl;

// 	return (0);
// }
