#pragma once

#include "webserv.hpp"

class LocationConfig
{
	public: 
		LocationConfig();
		LocationConfig(const LocationConfig &original);
		LocationConfig	&operator=(const LocationConfig &original);
		~LocationConfig();
		bool		operator==(const LocationConfig &original) const;

		class	MissingDirectiveExcept: public std::exception
		{
			public:
				const char *what() const throw() {
					return ("Missing a mandatory directive on the configuration file!");
				}
		};

		bool						exactMatch;
		std::string					locationPath;
		std::string					root;
		bool 						autoindex;
		std::vector<std::string>	allowed_methods;
		std::vector<std::string>	index;
		std::map<int, std::string>	returnDirective;
		std::string					cgi_extension;
		std::string					cgi_path;
		bool 						upload_enable;
		std::string					upload_store;
		int							client_max_body_size;

};
