#include "../include/webserv.hpp"


//Constructors
ConfigFileParser::ConfigFileParser()
{

}

ConfigFileParser::ConfigFileParser(const ConfigFileParser &original)
{

}

ConfigFileParser	&ConfigFileParser::operator=(const ConfigFileParser &original)
{
	return (*this);
}

ConfigFileParser::~ConfigFileParser()
{

}


//FUNCTIONS
void								ConfigFileParser::parseDirective(ServerConfig &server, const std::string &direction, const std::vector<std::string> &args)
{
	if (direction == "listen")
	{
		if (args.size() > 1)
		{
			throw IncorrectArgumentsExcept();
		}
		//listen 80
		if (args[0].find(':') == std::string::npos && args[0].find('.') == std::string::npos)
		{
			server.ip = "0.0.0.0";
			for (size_t i = 0; i < args[0].size(); i++)
			{
				if (!std::isdigit(args[0][i]))
				{
					throw IncorrectArgumentsExcept();
				}
			}
			server.port = atoi(args[0].c_str());
		}
		//listen 127.0.0.1
		else if (args[0].find(':') == std::string::npos && args[0].find('.') != std::string::npos)
		{
			int	numberOfBlocks = 1;
			int	i = 0;
			int	numberOfNums = 0;
	
			while (i < args[0].size())
			{
				if (numberOfNums == 3)
				{
					int	num = atoi(args[0].substr(i - 3, 3).c_str());
					if (num > 255)
					{
						throw IncorrectArgumentsExcept();
					}
				}
				if (numberOfNums > 3)
				{
					throw IncorrectArgumentsExcept();
				}
				if (args[0][i] == '.')
				{
					numberOfBlocks++;
					numberOfNums = 0;
				}
				else
				{
					if (!std::isdigit(args[0][i]))
					{
						throw IncorrectArgumentsExcept();
					}
					numberOfNums++;
				}
				i++;
			}
			if (numberOfBlocks != 4)
			{
				throw IncorrectArgumentsExcept();
			}
			server.ip = args[0];
			server.port = 80;
		}
		//listen  127.0.0.1:8080
		else
		{
			size_t 			dotPos = args[0].find('.');
			size_t 			doubleDotPos = args[0].find(':');

			if (dotPos > doubleDotPos)
			{
				throw IncorrectArgumentsExcept();
			}

			int	numberOfBlocks = 1;
			int	j = 0;
			int	numberOfNums = 0;
	
			while (j < doubleDotPos)
			{
				if (numberOfNums == 3)
				{
					int	num = atoi(args[0].substr(j - 3, 3).c_str());
					if (num > 255)
					{
						throw IncorrectArgumentsExcept();
					}
				}
				if (numberOfNums > 3)
				{
					throw IncorrectArgumentsExcept();
				}
				if (args[0][j] == '.')
				{
					numberOfBlocks++;
					numberOfNums = 0;
				}
				else
				{
					if (!std::isdigit(args[0][j]))
					{
						throw IncorrectArgumentsExcept();
					}
					numberOfNums++;
				}
				j++;
			}
			if (numberOfBlocks != 4)
			{
				throw IncorrectArgumentsExcept();
			}
			std::string		port;

			server.ip = args[0].substr(0, doubleDotPos);
			port = args[0].substr(doubleDotPos + 1);
			for (size_t i = 0; i < port.size(); i++)
			{
				if (!std::isdigit(port[i]))
				{
					throw IncorrectArgumentsExcept();
				}
			}
			server.port = atoi(port.c_str());
		}
	}
	else if (direction == "server_name")
	{
		if (args.size() > 1)
		{
			throw IncorrectArgumentsExcept();
		}
		server.server_name = args[0];
	}
	else if (direction == "root")
	{
		if (args.size() > 1)
		{
			throw IncorrectArgumentsExcept();
		}
		server.root = args[0];
	}
	else if (direction == "error_page")
	{
		if (args.size() < 2)
		{
			throw IncorrectArgumentsExcept();
		}
		for (size_t i = 0; i < args.size() - 1; i++)
		{
			int		error;
		
			for (size_t j = 0; j < args[i].size(); j++)
			{
				if (!std::isdigit(args[i][j]))
				{
					throw IncorrectArgumentsExcept();
				}
			}
			error = atoi(args[i].c_str());
			server.error_page[error] = args[args.size() - 1];
		}
	}
	else if (direction == "client_max_body_size")
	{
		if (args.size() > 1)
		{
			throw IncorrectArgumentsExcept();
		}
		for (size_t j = 0; j < args[0].size(); j++)
		{
			if (!std::isdigit(args[0][j]))
			{
				throw IncorrectArgumentsExcept();
			}
		}
		server.client_max_body_size = atoi(args[0].c_str());
	}
	//If direction == "index"
	else if (direction == "index")
	{
		for (size_t i = 0; i < args.size(); i++)
		{
			std::string		indexName = args[i];

			server.index.push_back(indexName);
		}
	}
	else 
	{
		throw InvalidDirectiveExcept();
	}
}


void								ConfigFileParser::parseDirective(LocationConfig &location, const std::string &direction, const std::vector<std::string> &args)
{
	if (direction == "root")
	{
		if (args.size() > 1)
		{
			throw IncorrectArgumentsExcept();
		}
		location.root = args[0];
	}
	else if (direction == "autoindex")
	{
		if (args.size() > 1)
		{
			throw IncorrectArgumentsExcept();
		}
		if (args[0] == "on")
		{
			location.autoindex = true;
		}
		else if (args[0] == "off")
		{
			location.autoindex = false;
		}
		else 
		{
			throw IncorrectArgumentsExcept();
		}
	}
	else if (direction == "allowed_methods")
	{
		if (args.size() > 3)
		{
			throw IncorrectArgumentsExcept();
		}
		for (size_t i = 0; i < args.size(); i++)
		{
			if (args[i] != "GET" && args[i] != "POST" && args[i] != "DELETE")
			{
				throw IncorrectArgumentsExcept();
			}
			else 
			{
				location.allowed_methods.push_back(args[i]);
			}
		}
	}
	else if (direction == "index")
	{
		for (size_t i = 0; i < args.size(); i++)
		{
			std::string		indexName = args[i];

			location.index.push_back(indexName);
		}
	}
	else if (direction == "return")
	{
		if (args.size() > 2)
		{
			throw IncorrectArgumentsExcept();
		}
		int	returnNumber;

		for (size_t j = 0; j < args[0].size(); j++)
		{
			if (!std::isdigit(args[0][j]))
			{
				throw IncorrectArgumentsExcept();
			}
		}
		returnNumber = atoi(args[0].c_str());
		if (args.size() == 1)
		{
			location.returnDirective[returnNumber] = nullptr;
		}
		else
		{
			location.returnDirective[returnNumber] = args[1];
		}
	}
	else if (direction == "cgi_extension")
	{
		if (args.size() > 1)
		{
			throw IncorrectArgumentsExcept();
		}
		location.cgi_extension = args[0];
	}
	else if (direction == "cgi_path")
	{
		if (args.size() > 1)
		{
			throw IncorrectArgumentsExcept();
		}
		location.cgi_path = args[0];
	}
	else if (direction == "upload_enable")
	{
		if (args.size() > 1)
		{
			throw IncorrectArgumentsExcept();
		}
		if (args[0] == "on")
		{
			location.upload_enable = true;
		}
		else if (args[0] == "off")
		{
			location.upload_enable = false;
		}
		else 
		{
			throw IncorrectArgumentsExcept();
		}
	}
	else if (direction == "upload_store")
	{
		if (args.size() > 1)
		{
			throw IncorrectArgumentsExcept();
		}
		location.upload_store = args[0];
	}
	else 
	{
		throw InvalidDirectiveExcept();
	}
}


bool								ConfigFileParser::isBlockStart(const std::string &token, const std::string block)
{
	if (block == "location")
	{
		return (false);
	}
	else if (block == "server" && token == "location")
	{
		return (true);
	}
	else 
	{
		return (false);
	}
}


bool								ConfigFileParser::isDirective(const std::string &token, const std::string block)
{
	if ((token == "listen" || token == "server_name" || token == "root" || token == "error_page" || token == "client_max_body_size" || token == "index") && block == "server")
	{
		//returns true if it is a server directive and we are in a server block. Else false
		return (true);
	}
	else if ((token == "root" || token == "autoindex" || token == "allowed_methods" || token == "index" || token == "return" || token == "cgi_extension" || token == "cgi_path" || token == "upload_enable" || token == "upload_store") && block == "location")
	{
		//returns true if it is a location directive and we are in a location block. Else false
		return (true);
	}
	else 
	{
		return (false);
	}
}


LocationConfig						ConfigFileParser::parseLocationBlock(const std::vector<std::string> &tokens, size_t &position)
{
	LocationConfig	location;

	//The first argument after the location blockStart is the location path.
	if (tokens[position + 2] == "{")
	{
		location.locationPath = tokens[position + 1];
		position = position + 3;
	}
	else if (tokens[position + 3] == "{")
	{
		if (tokens[position + 1] != "=" || (tokens[position + 2] == "{" || tokens[position + 2] == "}" || tokens[position + 2] == ";"))
		{
			throw UnexpectedTokenExcept();
		}
		location.exactMatch = true;
		location.locationPath = tokens[position + 1];
		position = position + 4;
	}
	while (position < tokens.size() && tokens[position] != "}")
	{
		bool directive = isDirective(tokens[position], "location");
		bool blockStart = isBlockStart(tokens[position], "location");

		if (!directive && !blockStart)
		{
			throw InvalidDirectiveExcept();
		}
		else 
		{
			if (directive)
			{
				std::vector<std::string>	args;
				std::string					directiveName;

				directiveName = tokens[position];
				position++;
				while (tokens[position] != ";")
				{
					if (tokens[position] == "{" || tokens[position] == "}")
					{
						throw UnexpectedTokenExcept();
					}
					args.push_back(tokens[position]);
					position++;
				}
				parseDirective(location, directiveName, args);
				position++;
			}
		}
	}
	//If position has reached the end of the vector and it has not been found an "}", returns an error
	if (position == tokens.size())
	{
		throw MissingCloseBracketExcept();
	}
	else 
	{
		position++;
		return (location);
	}
}


ServerConfig						ConfigFileParser::parseServerBlock(const std::vector<std::string> &tokens, size_t &position)
{
	ServerConfig	server;

	while (position < tokens.size() && tokens[position] != "}")
	{
		bool directive = isDirective(tokens[position], "server");
		bool blockStart = isBlockStart(tokens[position], "server");

		//Verificar que sea o una directiva o un comienzo de bloque (de location)
		if (!directive && !blockStart)
		{
			throw InvalidDirectiveExcept();
		}
		else 
		{
			if (blockStart)
			{
				//Si es un bloque de location, se crea una instance y se parsea para al final añadirla al vector de LocationConfig de la clase
				LocationConfig	location;

				if (tokens[position + 1] == "{" || tokens[position + 1] == "}" || tokens[position + 1] == ";")
				{
					throw InvalidLocationExcept();
				}
				location = parseLocationBlock(tokens, position);
				server.locations.push_back(location);
			}
			else if (directive)
			{
				//Si es una directiva, se comprueba que los argumentos sean correctos y se guardan para parsearse y guardarse en nuestro ServerConfig
				std::vector<std::string>	args;
				std::string					directiveName;

				directiveName = tokens[position];
				position++;
				while (tokens[position] != ";")
				{
					//Comprobamos que los argumentos sean válidos
					if (tokens[position] == "{" || tokens[position] == "}")
					{
						throw UnexpectedTokenExcept();
					}
					args.push_back(tokens[position]);
					position++;
				}
				parseDirective(server, directiveName, args);
				position++;
			}
		}
	}
	//If position has reached the end of the vector and it has not found a close bracket, it will give an error
	if (position == tokens.size())
	{
		throw MissingCloseBracketExcept();
	}
	else 
	{
		//After parsing the server and all the location blocks, we will give inherited values and default values in case some configurations are empty
		server.check();
		for (size_t i = 0; i < server.locations.size(); i++)
		{
			server.locations[i].check(server);
		}

		position++;
		return (server);
	}
}


void								ConfigFileParser::parseServers(const std::vector<std::string> &tokens, size_t &position, std::vector<ServerConfig> &servers)
{
	while (position < tokens.size())
	{
		//We create a server for each server block and we parse them
		if (tokens[position] == "server" && position + 1 < tokens.size() && tokens[position + 1] == "{")
		{
			ServerConfig	server;
	
			position = position + 2;
			//Parses the server and advances position to where (tokens[position - 1] == '}')
			server = parseServerBlock(tokens, position);
			servers.push_back(server);
		}
		else 
		{
			//If there is a token outside of serverBlocks and is not the server token, it will throw an error
			throw UnexpectedTokenExcept();
		}
	}
}


void								ConfigFileParser::tokenizer(const std::string &content, std::vector<std::string> &tokens)
{
	std::string		token;

	for (size_t i = 0; i < content.size(); i++)
	{
		//When a space is located, or a special character, the token created is pushed
		if (isspace(content[i]))
		{
			//If there was a token before the spaces, it is pushed to the vector
			if (!token.empty())
			{
				tokens.push_back(token);
				token.clear();
			}
		}
		else if (content[i] == '{' || content[i] == '}' || content[i] == ';')
		{
			//If there was a token before the special characters, it is pushed to the vector
			if (!token.empty())
			{
				tokens.push_back(token);
				token.clear();
			}
			//Also the special character is being treated as a token and it is pushed to the vector
			token = token + content[i];
			tokens.push_back(token);
			token.clear();
		}
		else 
		{
			//When there is no space nor special character, it means the token is still incomplete
			token = token + content[i];
		}
	}

	if (!token.empty())
	{
		//If it ends and it does not have a whitespace at the end, it won't push the token. We have to do it manually
		tokens.push_back(token);
	}
}


std::vector<ServerConfig>			ConfigFileParser::parse(const std::string &filename)
{
	//1. Read file into a string

	std::ifstream	configFile(filename);
	std::string		buffer;	//We store the file in here
	std::string		line;
	std::vector<std::string>	tokens; 	//We store the tokens in here
	std::vector<ServerConfig>	servers; 	//We store here the configuration of each of the servers
	size_t						position;	//For iterate the servers when parsing

	if (!configFile.is_open())
	{
		throw	InvalidFileExcept();
	}
	
	while (std::getline(configFile, line))
	{
		buffer.append(line);
	}
	configFile.close(); 

	//We already have the content of the config file in buffer

	//2. Tokenize the content and store it in a vector of token strings

	tokenizer(buffer, tokens);

	//We now have all the information tokenized

	//3. Parse Servers from tokens and store them in a vector of ServerConfig classes

	parseServers(tokens, position, servers);

	//Now we have the vector of servers ready to be returned

	return (servers);
}
