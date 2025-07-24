#pragma once

#include "webserv.hpp"

class ResponseBuilder
{
	public:
		ResponseBuilder();
		ResponseBuilder(const ResponseBuilder &original);
		ResponseBuilder		&operator=(const ResponseBuilder &original);
		~ResponseBuilder();

		HttpResponse	generateHttpResponse(const HttpRequest &request, const std::pair<ServerConfig, LocationConfig> &config);	//Given a request and server, builds a response. checks if method is allowed and returns an HttpResponse object
		std::string		getPath(const bool exactMatch, const std::string method, const std::string serverRoot, const std::string root, const std::string locPrefix, const std::string uri, const std::vector<std::string> index, const bool autoindex);
		std::string		generateFilename(const std::string &mime);
		std::string		getExtensionType(const std::string &mime);
		std::string 	generateAutoIndexHtml(const std::string& dirPath);

		class	InvalidMethodExcept: public std::exception
		{
			public:
				const char *what() const throw() {
					return ("Method is not allowed!");
				}
		};

		class	InvalidIndexExcept: public std::exception
		{
			public:
				const char *what() const throw() {
					return ("Invalid index!");
				}
		};

		class	InvalidPathExcept: public std::exception
		{
			public:
				const char *what() const throw() {
					return ("Invalid path!");
				}
		};

	private:
		std::string		getFileContent(const std::string &path);	//Reads the content of a  file from our disk and returns the content as a string. For GET request
		std::string		getMimeType(const std::string &path);	//Given a file extension, returns the correct MIME type for the Content-Type header in response

};
