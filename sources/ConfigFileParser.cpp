#include "../include/webserv.hpp"


//Constructors
ConfigFileParser::ConfigFileParser()
{

}

ConfigFileParser::ConfigFileParser(const ConfigFileParser &original)
{
	*this = original;
}

ConfigFileParser	&ConfigFileParser::operator=(const ConfigFileParser &original)
{
	if (this != &original)
	{
	
	}
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
		if (args.size() != 1)
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
			if (server.port < 1 || server.port > 65535)
			{
				throw IncorrectArgumentsExcept();
			}
		}
		//listen 127.0.0.1
		else if (args[0].find(':') == std::string::npos && args[0].find('.') != std::string::npos)
		{
			int	numberOfBlocks = 1;
			size_t	i = 0;
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
					if (!std::isdigit(args[0][i + 1]))
					{
						throw IncorrectArgumentsExcept();
					}
					if (numberOfNums == 0)
					{
						throw IncorrectArgumentsExcept();
					}
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
			if (numberOfNums == 3)
			{
				int	num = atoi(args[0].substr(i - 3, 3).c_str());
				if (num > 255)
				{
					throw IncorrectArgumentsExcept();
				}
			}
			// std::cout << numberOfBlocks <<  std::endl;
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

			if (!std::isdigit(args[0][args[0].find(':') + 1]))
			{
				throw IncorrectArgumentsExcept();
			}

			if (dotPos > doubleDotPos)
			{
				throw IncorrectArgumentsExcept();
			}

			int	numberOfBlocks = 1;
			size_t	j = 0;
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
					if (!std::isdigit(args[0][j + 1]))
					{
						throw IncorrectArgumentsExcept();
					}
					if (numberOfNums == 0)
					{
						throw IncorrectArgumentsExcept();
					}
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
			if (numberOfNums == 3)
			{
				int	num = atoi(args[0].substr(j - 3, 3).c_str());
				if (num > 255)
				{
					throw IncorrectArgumentsExcept();
				}
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
			if (server.port < 1 || server.port > 65535)
			{
				throw IncorrectArgumentsExcept();
			}
		}
	}
	else if (direction == "server_name")
	{
		if (args.size() != 1)
		{
			throw IncorrectArgumentsExcept();
		}
		server.server_name = args[0];
	}
	else if (direction == "root")
	{
		if (!server.root.empty())
		{
			throw IncorrectArgumentsExcept();
		}
		if (args.size() != 1)
		{
			throw IncorrectArgumentsExcept();
		}
		server.root = args[0];
		if (server.root[0] != '/')
		{
			throw IncorrectArgumentsExcept();
		}
		if (server.root.size() >= 3 && server.root[1] == '.' && server.root[2] == '.')
		{
			throw IncorrectArgumentsExcept();
		}
		if (!Utils::isDirectory(server.root))
		{
			throw IncorrectArgumentsExcept();
		}
	}
	else if (direction == "autoindex")
	{
		if (args.size() != 1)
		{
			throw IncorrectArgumentsExcept();
		}
		if (args[0] == "on")
		{
			server.autoindex = true;
		}
		else if (args[0] == "off")
		{
			server.autoindex = false;
		}
		else 
		{
			throw IncorrectArgumentsExcept();
		}
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
		size_t k;
		if (args.size() != 1)
		{
			throw IncorrectArgumentsExcept();
		}
		for (k = 0; k < args[0].size() - 1; k++)
		{
			if (!std::isdigit(args[0][k]))
			{
				throw IncorrectArgumentsExcept();
			}
		}
		if (std::isdigit(args[0][k]))
		{
			server.client_max_body_size = atoi(args[0].c_str());
		}
		else if (args[0][k] == 'K' && args[0].size() > 1)
		{
			std::string	body_size = args[0].substr(0, args[0].size() - 1);
			server.client_max_body_size = atoi(body_size.c_str()) * 1024;
		}
		else if (args[0][k] == 'M' && args[0].size() > 1)
		{
			std::string	body_size = args[0].substr(0, args[0].size() - 1);
			server.client_max_body_size = atoi(body_size.c_str()) * 1024 * 1024;
		}
		else 
		{
			throw IncorrectArgumentsExcept();
		}
	}
	//If direction == "index"
	else if (direction == "index")
	{
		if (args.size() < 1)
		{
			throw IncorrectArgumentsExcept();
		}
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
		if (args.size() != 1)
		{
			throw IncorrectArgumentsExcept();
		}
		location.root = args[0];
		if (location.root[0] != '/')
		{
			throw IncorrectArgumentsExcept();
		}
		if (location.root.size() >= 3 && location.root[1] == '.' && location.root[2] == '.')
		{
			throw IncorrectArgumentsExcept();
		}

		if (!Utils::isDirectory(location.root))
		{
			throw IncorrectArgumentsExcept();
		}
	}
	else if (direction == "client_max_body_size")
	{
		size_t k;
		if (args.size() != 1)
		{
			throw IncorrectArgumentsExcept();
		}
		for (k = 0; k < args[0].size() - 1; k++)
		{
			if (!std::isdigit(args[0][k]))
			{
				throw IncorrectArgumentsExcept();
			}
		}
		if (std::isdigit(args[0][k]))
		{
			location.client_max_body_size = atoi(args[0].c_str());
		}
		else if (args[0][k] == 'K' && args[0].size() > 1)
		{
			std::string	body_size = args[0].substr(0, args[0].size() - 1);
			location.client_max_body_size = atoi(body_size.c_str()) * 1024;
		}
		else if (args[0][k] == 'M' && args[0].size() > 1)
		{
			std::string	body_size = args[0].substr(0, args[0].size() - 1);
			location.client_max_body_size = atoi(body_size.c_str()) * 1024 * 1024;
		}
		else 
		{
			throw IncorrectArgumentsExcept();
		}
	}
	else if (direction == "autoindex")
	{
		if (args.size() != 1)
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
		if (args.size() > 3 || args.size() == 0)
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
				bool	methodAlready = false;
				for (size_t j = 0; j < location.allowed_methods.size(); j++)
				{
					if (args[i] == location.allowed_methods[j])
					{
						methodAlready = true;
						break;
					}
				}
				if (!methodAlready)
				{
					location.allowed_methods.push_back(args[i]);
				}
			}
		}
	}
	else if (direction == "index")
	{
		if (args.size() < 1)
		{
			throw IncorrectArgumentsExcept();
		}
		for (size_t i = 0; i < args.size(); i++)
		{
			std::string		indexName = args[i];

			location.index.push_back(indexName);
		}
	}
	else if (direction == "return")
	{
		if (args.size() > 2 || args.size() < 1)
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
			location.returnDirective[returnNumber] = "";
		}
		else
		{
			location.returnDirective[returnNumber] = args[1];
		}
	}
	else if (direction == "cgi_extension")
	{
		if (args.size() != 1)
		{
			throw IncorrectArgumentsExcept();
		}
		location.cgi_extension = args[0];
	}
	else if (direction == "cgi_path")
	{
		if (args.size() != 1)
		{
			throw IncorrectArgumentsExcept();
		}
		location.cgi_path = args[0];
	}
	else if (direction == "upload_enable")
	{
		if (args.size() != 1)
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
		if (args.size() != 1)
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
	if ((token == "listen" || token == "server_name" || token == "root" || token == "error_page" || token == "client_max_body_size" || token == "index" || token == "autoindex") && block == "server")
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


LocationConfig						ConfigFileParser::parseLocationBlock(const std::vector<std::string> &tokens, size_t &position, std::vector<LocationConfig> &locations)
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
		location.locationPath = tokens[position + 2];
		if (location.locationPath[location.locationPath.size() - 1] == '/')
		{
			throw InvalidLocationExcept();
		}
		position = position + 4;
	}
	else 
	{
		throw InvalidLocationExcept();
	}
	for (size_t i = 0; i < locations.size(); i++)
	{
		// std::cout <<locations[i].locationPath << std::endl;
		// std::cout << location.locationPath << std::endl;
		if (location.locationPath == locations[i].locationPath)
		{
			if (location.exactMatch)
			{
				if (!locations[i].exactMatch)
				{
					continue;
				}
			}
			else if (locations[i].exactMatch)
			{
				if (!location.exactMatch)
				{
					continue;
				}
			}
			locations.erase(locations.begin() + i);
			break;
		}
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
				location = parseLocationBlock(tokens, position, server.locations);
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
		bool	defaultPath = false;
		for (size_t i = 0; i < server.locations.size(); i++)
		{
			if (server.locations[i].locationPath == "/")
			{
				defaultPath = true;
				break;
			}
		}
		if (!defaultPath || server.locations.size() == 0)
		{
			LocationConfig	locationDef;
			locationDef.locationPath = "/";
			server.locations.push_back(locationDef);
		}
		for (size_t i = 0; i < server.locations.size(); i++)
		{
			server.check(server, server.locations[i]);
			if (server.locations[i].locationPath[0] != '/')
			{
				throw InvalidLocationExcept();
			}
			for (size_t j = 0; j < server.locations.size(); j++)
			{
				if (j != i)
				{
					if (server.locations[i].locationPath == server.locations[j].locationPath)
					{
						if ((server.locations[i].exactMatch && !server.locations[j].exactMatch) || (!server.locations[i].exactMatch && server.locations[j].exactMatch))
						{
							continue;
						}
						throw InvalidLocationExcept();
					}
				}
			}
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
			if (tokens[position] == "}")
			{
				throw UnexpectedTokenExcept();
			}
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

	std::ifstream	configFile(filename.c_str());
	std::string		buffer;	//We store the file in here
	std::string		line;
	std::vector<std::string>	tokens; 	//We store the tokens in here
	std::vector<ServerConfig>	servers; 	//We store here the configuration of each of the servers
	size_t						position = 0;	//For iterate the servers when parsing

	if (!configFile.is_open())
	{
		throw	InvalidFileExcept();
	}
	while (std::getline(configFile, line))
	{
		buffer.append(line);
		buffer.append("\n");
	}
	size_t pos = buffer.find("#");
	while (pos != std::string::npos)
	{
		size_t npos = buffer.find("\n", pos);
		buffer = buffer.erase(pos, npos - pos);
		pos = buffer.find("#");
	}
	configFile.close(); 
	//debugging:
	// std::cout << buffer << std::endl;

	//We already have the content of the config file in buffer

	//2. Tokenize the content and store it in a vector of token strings

	tokenizer(buffer, tokens);
	//debugging:
	// for (size_t i = 0; i < tokens.size(); i++)
	// {
	// 	std::cout << tokens[i] << std::endl;
	// }

	//We now have all the information tokenized

	//3. Parse Servers from tokens and store them in a vector of ServerConfig classes

	parseServers(tokens, position, servers);
	if (servers.size() == 0)
	{
		throw	InvalidFileExcept();
	}
	//Now we have the vector of servers ready to be returned

	return (servers);
}
