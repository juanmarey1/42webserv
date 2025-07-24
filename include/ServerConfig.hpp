#pragma once

#include "webserv.hpp"

class ServerConfig
{
	public:
		ServerConfig();
		ServerConfig(const ServerConfig &original);
		ServerConfig	&operator=(const ServerConfig &original);
		~ServerConfig();
		bool		operator==(const ServerConfig &original);

		void 		check();
		void		check(const ServerConfig &server, LocationConfig &location);

		class	MissingDirectiveExcept: public std::exception
		{
			public:
				const char *what() const throw() {
					return ("Missing a mandatory directive on the configuration file!");
				}
		};

		std::vector<LocationConfig>		locations;
		std::string						ip;
		int 							port;
		std::string 					server_name;
		std::string						root;
		std::map<int, std::string>		error_page;
		int 							client_max_body_size;
		std::vector<std::string>		index;
		bool							autoindex;
};
