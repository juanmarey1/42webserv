#pragma once

#include "webserv.hpp"

class ServerManager 
{
	private:

		std::vector<ServerConfig>				serversList; 			//Vector containing the information of each server
		std::map<int, ServerConfig>				listeningSockets;		//We have a socket FD for each server with unique (Host, Port)
		std::map<int, ConnectionHandler*>		clientConnections;		//We have a conection for each client socket FD
		std::vector<pollfd>						pollfds;				//Vector containing a list of pollfd structs. Each one represents a socket FD that poll() monitors for events

		void		acceptNewClient(int server_fd);				//Accepts new client
		void		handleClientRequest(int client_fd);			//Handles client requests
		void		closeClient(int client_fd);					//Closes conection with a client


	public: 

		ServerManager();
		ServerManager(const ServerManager &original);
		ServerManager	&operator=(const ServerManager &original);
		~ServerManager();

		void		loadConfigParsing(const std::string &filename);		//Starts the parsing of the config file
		void		initSockets();										//Initializes each of the sockets
		void		run();												//Main loop for poll()

};
