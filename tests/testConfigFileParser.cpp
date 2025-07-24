// #include "../include/webserv.hpp"


// int main()
// {
// 	//TEST1
// 	ConfigFileParser	configParser;
// 	std::cout << "\nTEST 1 ....................................... SINGLE SERVER\n" << std::endl;
// 	std::vector<ServerConfig>	servers = configParser.parse("/home/jrey-roj/juanma/webserv_nogit/tests/var/www/configFile1.config");
// 	for (size_t i = 0; i < servers.size(); i++)
// 	{
// 		std::cout << "SERVER: " << i << std::endl;
// 		std::cout << "ip: " << servers[i].ip << std::endl;
// 		std::cout << "port: " << servers[i].port << std::endl;
// 		std::cout << "servername: " << servers[i].server_name << std::endl;
// 		std::cout << "root: " << servers[i].root << std::endl;
// 		std::cout << "client_max_body_size: " << servers[i].client_max_body_size << std::endl;
// 		if (!servers[i].index.empty())
// 		{
// 			for (size_t j = 0; j < servers[i].index.size(); j++)
// 			{
// 				std::cout << "index " << j << ": " << servers[i].index[j] << std::endl;
// 			}
// 		}
// 		if (!servers[i].error_page.empty())
// 		{
// 			for (std::map<int,  std::string>::iterator it = servers[i].error_page.begin(); it != servers[i].error_page.end(); it++) {
//          		std::cout  << "error page " << it->first << ": " << it->second << std::endl; 
//      		}
// 		}
// 	}

// 	std::cout << "\nSUCCESSSSS  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n" << std::endl;

// 	ConfigFileParser	configParser2;
// 	std::cout << "\nTEST 2 ....................................... MULTIPLE SERVER\n" << std::endl;
// 	std::vector<ServerConfig>	servers2 = configParser2.parse("/home/jrey-roj/juanma/webserv_nogit/tests/var/www/configFile2.config");
// 	for (size_t i = 0; i < servers2.size(); i++)
// 	{
// 		std::cout << "SERVER: " << i + 1 << std::endl;
// 		std::cout << "ip: " << servers2[i].ip << std::endl;
// 		std::cout << "port: " << servers2[i].port << std::endl;
// 		std::cout << "servername: " << servers2[i].server_name << std::endl;
// 		std::cout << "root: " << servers2[i].root << std::endl;
// 		std::cout << "client_max_body_size: " << servers2[i].client_max_body_size << std::endl;
// 		if (!servers2[i].index.empty())
// 		{
// 			for (size_t j = 0; j < servers2[i].index.size(); j++)
// 			{
// 				std::cout << "index " << j << ": " << servers2[i].index[j] << std::endl;
// 			}
// 		}
// 		if (!servers2[i].error_page.empty())
// 		{
// 			for (std::map<int,  std::string>::iterator it = servers2[i].error_page.begin(); it != servers2[i].error_page.end(); it++) {
//          		std::cout  << "error page " << it->first << ": " << it->second << std::endl; 
//      		}
// 		}
// 		std::cout << std::endl;
// 	}

// 	std::cout << "\nSUCCESSSSS  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n" << std::endl;

// 	ConfigFileParser	configParser3;
// 	std::cout << "\nTEST 3 ....................................... server with location\n" << std::endl;
// 	std::vector<ServerConfig>	servers3 = configParser3.parse("/home/jrey-roj/juanma/webserv_nogit/tests/var/www/configFile3.config");
// 	for (size_t i = 0; i < servers3.size(); i++)
// 	{
// 		std::cout << "SERVER: " << i + 1 << std::endl;
// 		std::cout << "ip: " << servers3[i].ip << std::endl;
// 		std::cout << "port: " << servers3[i].port << std::endl;
// 		std::cout << "servername: " << servers3[i].server_name << std::endl;
// 		std::cout << "root: " << servers3[i].root << std::endl;
// 		std::cout << "client_max_body_size: " << servers3[i].client_max_body_size << std::endl;
// 		if (!servers3[i].index.empty())
// 		{
// 			for (size_t j = 0; j < servers3[i].index.size(); j++)
// 			{
// 				std::cout << "index " << j << ": " << servers3[i].index[j] << std::endl;
// 			}
// 		}
// 		if (!servers3[i].error_page.empty())
// 		{
// 			for (std::map<int,  std::string>::iterator it = servers3[i].error_page.begin(); it != servers3[i].error_page.end(); it++) {
//          		std::cout  << "error page " << it->first << ": " << it->second << std::endl; 
//      		}
// 		}
// 		if (!servers3[i].locations.empty())
// 		{
// 			for (size_t j = 0; j < servers3[i].locations.size(); j++)
// 			{
// 				std::cout << "\nLOCATION: " << j + 1 << std::endl;
// 				LocationConfig	location = servers3[i].locations[j];
// 				std::cout << "exactmatch: " << location.exactMatch << std::endl;
// 				std::cout << "locationPath: " << location.locationPath << std::endl;
// 				std::cout << "root: " << location.root << std::endl;
// 				std::cout << "autoindex: " << location.autoindex << std::endl;
// 				std::cout << "cgi_extension: " << location.cgi_extension << std::endl;
// 				std::cout << "cgi_path: " << location.cgi_path << std::endl;
// 				std::cout << "upload_enable: " << location.upload_enable << std::endl;
// 				std::cout << "upload_store" << location.upload_store << std::endl;
// 				if (!location.index.empty())
// 				{
// 					for (size_t k = 0; k < location.index.size(); k++)
// 					{
// 						std::cout << "index " << k << ": " << location.index[k] << std::endl;
// 					}
// 				}
// 				if (!location.allowed_methods.empty())
// 				{
// 					for (size_t k = 0; k < location.allowed_methods.size(); k++)
// 					{
// 						std::cout << "allowed_methods " << k << ": " << location.allowed_methods[k] << std::endl;
// 					}
// 				}
// 				if (!location.returnDirective.empty())
// 				{
// 					for (std::map<int,  std::string>::iterator it = location.returnDirective.begin(); it != location.returnDirective.end(); it++) {
//          				std::cout  << "return directive " << it->first << ": " << it->second << std::endl; 
//      				}
// 				}

// 			}
// 		}
// 		std::cout << std::endl;
// 	}

// 	std::cout << "\nSUCCESSSSS  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n" << std::endl;

// 	ConfigFileParser	configParser4;
// 	std::cout << "\nTEST 4 ....................................... multiple nested location\n" << std::endl;
// 	std::vector<ServerConfig>	servers4 = configParser4.parse("/home/jrey-roj/juanma/webserv_nogit/tests/var/www/configFile4.config");
// 	for (size_t i = 0; i < servers4.size(); i++)
// 	{
// 		std::cout << "SERVER: " << i + 1 << std::endl;
// 		std::cout << "ip: " << servers4[i].ip << std::endl;
// 		std::cout << "port: " << servers4[i].port << std::endl;
// 		std::cout << "servername: " << servers4[i].server_name << std::endl;
// 		std::cout << "root: " << servers4[i].root << std::endl;
// 		std::cout << "client_max_body_size: " << servers4[i].client_max_body_size << std::endl;
// 		if (!servers4[i].index.empty())
// 		{
// 			for (size_t j = 0; j < servers4[i].index.size(); j++)
// 			{
// 				std::cout << "index " << j << ": " << servers4[i].index[j] << std::endl;
// 			}
// 		}
// 		if (!servers4[i].error_page.empty())
// 		{
// 			for (std::map<int,  std::string>::iterator it = servers4[i].error_page.begin(); it != servers4[i].error_page.end(); it++) {
//          		std::cout  << "error page " << it->first << ": " << it->second << std::endl; 
//      		}
// 		}
// 		if (!servers4[i].locations.empty())
// 		{
// 			for (size_t j = 0; j < servers4[i].locations.size(); j++)
// 			{
// 				std::cout << "\nLOCATION: " << j + 1 << std::endl;
// 				LocationConfig	location = servers4[i].locations[j];
// 				std::cout << "exactmatch: " << location.exactMatch << std::endl;
// 				std::cout << "locationPath: " << location.locationPath << std::endl;
// 				std::cout << "root: " << location.root << std::endl;
// 				std::cout << "autoindex: " << location.autoindex << std::endl;
// 				std::cout << "cgi_extension: " << location.cgi_extension << std::endl;
// 				std::cout << "cgi_path: " << location.cgi_path << std::endl;
// 				std::cout << "upload_enable: " << location.upload_enable << std::endl;
// 				std::cout << "upload_store" << location.upload_store << std::endl;
// 				if (!location.index.empty())
// 				{
// 					for (size_t k = 0; k < location.index.size(); k++)
// 					{
// 						std::cout << "index " << k << ": " << location.index[k] << std::endl;
// 					}
// 				}
// 				if (!location.allowed_methods.empty())
// 				{
// 					for (size_t k = 0; k < location.allowed_methods.size(); k++)
// 					{
// 						std::cout << "allowed_methods " << k << ": " << location.allowed_methods[k] << std::endl;
// 					}
// 				}
// 				if (!location.returnDirective.empty())
// 				{
// 					for (std::map<int,  std::string>::iterator it = location.returnDirective.begin(); it != location.returnDirective.end(); it++) {
//          				std::cout  << "return directive " << it->first << ": " << it->second << std::endl; 
//      				}
// 				}

// 			}
// 		}
// 		std::cout << std::endl;
// 	}

// 	std::cout << "\nSUCCESSSSS  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n" << std::endl;


// 	return (0);
// }
