#pragma once

#include "webserv.hpp"

class ConfigFileParser
{
	public:

		ConfigFileParser();																			
		ConfigFileParser(const ConfigFileParser &original);
		ConfigFileParser	&operator=(const ConfigFileParser &original);
		~ConfigFileParser();

		std::vector<ServerConfig>	parse(const std::string &filename);		//Reads the entire file, tokenizes, parses servers and returns the servers vector to the ServerManager class


		class	InvalidFileExcept: public std::exception
		{
			public:
				const char *what() const throw() {
					return ("Cannot open this configuration file!");
				}
		};

		class	UnexpectedTokenExcept: public std::exception
		{
			public:
				const char *what() const throw() {
					return ("Token should not be here!");
				}
		};

		class	MissingCloseBracketExcept: public std::exception
		{
			public:
				const char *what() const throw() {
					return ("Close bracket missing!");
				}
		};

		class	InvalidDirectiveExcept: public std::exception
		{
			public:
				const char *what() const throw() {
					return ("Incorrect directive!");
				}
		};

		class	InvalidLocationExcept: public std::exception
		{
			public:
				const char *what() const throw() {
					return ("Invalid location block!");
				}
		};

		class	IncorrectArgumentsExcept: public std::exception
		{
			public:
				const char *what() const throw() {
					return ("Invalid arguments for the directory!");
				}
		};

	private:

		void						tokenizer(const std::string &content, std::vector<std::string> &tokens);											//Splits the config text into tokens and stores them in a vector
		void 						parseServers(const std::vector<std::string> &tokens, size_t &position, std::vector<ServerConfig> &servers);			//Looks for server blocks and calls parseServerBlock() on each one. After that, it will push it to the vector of servers

		ServerConfig 				parseServerBlock(const std::vector<std::string> &tokens, size_t &position);											//Parses server-level directives. Parses location blocks with parseLocationBlock(). Returns a ServerConfig object
		LocationConfig				parseLocationBlock(const std::vector<std::string> &tokens, size_t &position, std::vector<LocationConfig> &locations);										//Parses location-level directives. Returns a LocationConfig object

		void						parseDirective(ServerConfig &server, const std::string &direction, const std::vector<std::string> &args);			//Extracts directive name and its args and stores them. It checks if the values are correct.
		void 						parseDirective(LocationConfig &location, const std::string &direction, const std::vector<std::string> &args);		

		bool						isDirective(const std::string &token, const std::string block);																				//Checks if the token we passed matches the name of a directive/block start
		bool						isBlockStart(const std::string &token, const std::string block);

};
