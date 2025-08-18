#include "../include/webserv.hpp"

//Constructors, operators and destructor

ServerManager::ServerManager() {
	this->acceptedConections.clear();
}

ServerManager::ServerManager(const ServerManager &original)
{
	this->serversList = original.serversList;
	this->listeningSockets = original.listeningSockets;
	this->clientConnections = original.clientConnections;
	this->pollfds = original.pollfds;
	this->acceptedConections = original.acceptedConections;
}

ServerManager	&ServerManager::operator=(const ServerManager &original)
{
	if (this != &original)
	{
		this->serversList = original.serversList;
		this->listeningSockets = original.listeningSockets;
		this->clientConnections = original.clientConnections;
		this->pollfds = original.pollfds;
		this->acceptedConections = original.acceptedConections;
	}
	return (*this);
}

ServerManager::~ServerManager()
{
	if (!this->clientConnections.empty())
	{
		//We free the memory allocated for each of the conections:
		std::map<int, ConnectionHandler*>::iterator		it;

		for (it = this->clientConnections.begin(); it != this->clientConnections.end(); ++it)
		{
			if (it->second)
			{
				delete it->second;
			}
		}

	}

	//We close each of the FDs that were waiting for poll()
	for (size_t i = 0; i < this->pollfds.size(); ++i)
	{
		if (this->pollfds[i].fd >= 0)
		{
			close(this->pollfds[i].fd);
			this->pollfds[i].fd = -1;
		}
	}
}


//Functions
void			ServerManager::loadConfigParsing(const std::string &filename)
{
	//We parse the servers in the function ConfigFileParse::parse() and we get them as a vector to store them
	ConfigFileParser		configParser;
	
	this->serversList = configParser.parse(filename);
	std::cout << "[INFO] - CONFIGURATION FILE PARSED!" << std::endl;
}


void			ServerManager::initSockets()
{
	//We create a new pair for each unique hostPort pair.
	std::vector<std::pair<std::string, int> >	hostPortList;
	for (size_t i = 0; i < this->serversList.size(); i++)
	{
		std::pair<std::string, int>		hostPort;
		bool							pushToList = true;

		hostPort.first = this->serversList[i].ip;
		hostPort.second = this->serversList[i].port;
		for (size_t j = 0; j < hostPortList.size(); j++)
		{
			if (hostPort.first == hostPortList[j].first && hostPort.second == hostPortList[j].second)
			{
				pushToList = false;
				break;
			}
		}
		if (pushToList == true)
		{
			hostPortList.push_back(hostPort);

			//For each unique pair:
			//Create the socket
			int		sockfd = socket(AF_INET, SOCK_STREAM, 0);
			//Set socket options
			int 	opt = 1;
			setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
			fcntl(sockfd, F_SETFL, O_NONBLOCK);
			//Prepare sockaddr_in
			sockaddr_in 	addr;
			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(hostPort.second);
			inet_pton(AF_INET, hostPort.first.c_str(), &addr.sin_addr);
			//We bind the socket
			if (bind(sockfd, (sockaddr*)&addr, sizeof(addr)) < 0)
			{
				if (errno == EADDRINUSE)
				{
					throw std::runtime_error("Port already in use");
				}
				else 
				{
					throw std::runtime_error("Bind failed!");
				}
			}
			//We listen for incoming connections
			listen(sockfd, SOMAXCONN);

			//We store the socket FD and the related config in a map
			this->listeningSockets[sockfd] = this->serversList[i];
			std::cout << "[INFO] - INITIALIZING SOCKET ON " << hostPort.first << ":" << hostPort.second << std::endl;
			//We add pollfd struct to pollfds vector with the event flag POLLIN (waits for a readable envent)
			pollfd	pfd;
			pfd.fd = sockfd;
			pfd.events = POLLIN;
			pfd.revents = 0;
			this->pollfds.push_back(pfd);
		}
	}
	//DEBUGGING: 
	// for (size_t i = 0; i < hostPortList.size(); i++)
	// {
	// 	std::cout << hostPortList[i].first << std::endl;
	// 	std::cout << hostPortList[i].second << "\n" <<std::endl;
	// }
	// for (std::map<int, ServerConfig>::iterator it = this->listeningSockets.begin(); it != this->listeningSockets.end(); it++)
	// {
	// 	std::cout << it->first << std::endl;
	// 	std::cout << it->second.root << std::endl;
	// }
	std::cout << "[INFO] - SERVER IS READY TO RUN!" << std::endl;
}


void			ServerManager::run()
{
	//Runs the main loop of poll()
	std::cout << "[INFO] - SERVER IS RUNNING!" << std::endl;
	while (true)
	{
		int	timeout = 10 * 1000;
		// Returns the number of ready sockets, or 0 on timeout, or -1 on error
		//DEBUGGING:
		// std::cout << "current pollfds: " << std::endl;
		// for (size_t i = 0; i < this->pollfds.size(); i++)
		// {
		// 	std::cout << this->pollfds[i].fd << " " << this->pollfds[i].revents << std::endl;
		// }
		// std::cout << "" << std::endl;
		////
		int	ready = poll(this->pollfds.data(), this->pollfds.size(), timeout);
		// std::cout << "Iniciando poll()\n" << std::endl;
		if (ready <= 0)
		{
			std::time_t now = std::time(NULL);
			if (ready == 0)
			{
				for (std::map<int, ConnectionHandler *>::iterator it = this->clientConnections.begin(); it != this->clientConnections.end(); it++)
				{
					if (now - it->second->lastActivity > TIMEOUT)
					{
						std::cout << "[INFO] - Closing idle connection (fd = " << it->first << ")" << std::endl;
						closeClient(it->first);
					}
				}
			}
			continue;
		}
		for (size_t i = 0; i < this->pollfds.size(); i++)
		{
			//If no events skip
			if (this->pollfds[i].revents == 0)
			{
				//DEBUGGING:
				// std::cout << i << " " << this->pollfds[i].events << std::endl;
				// std::cout << this->pollfds[i].revents << std::endl;
				continue;
			}

			int fd = this->pollfds[i].fd;
			// std::cout << "NEW LOOP: " << i + 1 <<std::endl;
			// bool sexo = this->pollfds[i].revents & POLLIN;
			// std::cout << this->pollfds[i].fd << " " << this->pollfds[i].revents << " eventts: " << sexo << std::endl;
			// std::cout << "" << std::endl;
			

			//check if socket has data to read
			if (this->pollfds[i].revents & POLLIN)
			{
				//If it is a server socket (found in the map) -> New client trying to connect. acceptClient(fd) to accept connection and add new client
				if (this->listeningSockets.count(fd))
				{
					// std::cout << fd << std::endl;
					acceptNewClient(fd);
				// //DEBUGGING:
				// std::cout << "sexy accept" << std::endl;
				// for (size_t i = 0; i < this->pollfds.size(); i++)
				// {
				// 	std::cout << this->pollfds[i].fd << " " << this->pollfds[i].revents << std::endl;
				// }
				// std::cout << "end of accept" << std::endl;
				}
				
				//If it is a client socket (not found in the map) -> Call handleClient to read data, parse requests, etc.
				else 
				{
					// std::cout << fd << std::endl;
					handleClientRequest(fd);
					// //DEBUGGING:
					// for (size_t i = 0; i < this->pollfds.size(); i++)
					// {
					// 	std::cout << this->pollfds[i].fd << " " << this->pollfds[i].revents << std::endl;
					// }
					// //////
				}

			}
			else if (this->pollfds[i].revents & POLLOUT)
			{
				handleClientRequest(fd);
			}
			else
			{
				closeClient(fd);
			}
		}
	}

}


void			ServerManager::acceptNewClient(int server_fd)
{
	//Accept incoming connection and returns a new socket descriptor for client
	while (true)
	{
		int	client_fd = accept(server_fd, NULL, NULL);
		if (client_fd < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
					break;
			}
			break;
		}
		//Set the new socket to non-blocking mode
		fcntl(client_fd, F_SETFL, O_NONBLOCK);
		//Creates a new ConnectionHandler object to track client state. Store in clients map
		this->clientConnections[client_fd] = new ConnectionHandler(client_fd);
		this->clientToServerMap[client_fd] = this->listeningSockets[server_fd];
		for (size_t i = 0; i < this->pollfds.size(); i++)
		{
			if (server_fd == this->pollfds[i].fd)
			{
				this->pollfds[i].revents = 0;
				break;
			}
		}
		//Add client socket to pollfds vector and marks it for reading
		pollfd	fds;
		fds.fd = client_fd;
		fds.events = POLLIN;
		fds.revents = 0;
		this->pollfds.push_back(fds);
	// // //DEBUGGING:
	// std::cout << "sexy accept" << std::endl;
	// 	for (size_t i = 0; i < this->pollfds.size(); i++)
	// 	{
	// 		std::cout << this->pollfds[i].fd << " " << this->pollfds[i].revents << std::endl;
	// 	}
	// 	std::cout << "end of accept" << std::endl;
	// 	//////
	}
	
}


void			ServerManager::handleClientRequest(int client_fd)
{
	//If state == READING, keep reading the request. Only close connection in case that there was a timeout or error
	if (!this->clientConnections[client_fd]->isRequestComplete())
	{
		if (!this->clientConnections[client_fd]->readRequest())
		{
			closeClient(client_fd);
			return;
		}
		//If we have completed our request, route it and at the end change the poll.events to POLLOUT
		if (this->clientConnections[client_fd]->isRequestComplete())
		{
			//We route it to the most precise server and location
			//First we get a vector of all the servers that match the same ip and port of the current listening socket with this fd.
			Router			router;
			std::string		ip;
			int				port;
			std::vector<ServerConfig>	matchServers;
			
			ip = this->clientToServerMap[client_fd].ip;
			port = this->clientToServerMap[client_fd].port;
			//DEBUGGING:
			// std::cout << ip << std::endl;
			// std::cout << port << std::endl;
			for (size_t i = 0; i < this->serversList.size(); i++)
			{
				if (this->serversList[i].port == port && this->serversList[i].ip == ip)
				{
					matchServers.push_back(this->serversList[i]);
				}
			}
			//When we got our list of servers that match ip and port, we pass it for routing it.
			this->clientConnections[client_fd]->config = router.route(this->clientConnections[client_fd]->request, matchServers);

			//After routing it when it finishes parsing request, we change its state to POLLOUT
			for (size_t i = 0; i < this->pollfds.size(); i++)
			{
				if (client_fd == this->pollfds[i].fd)
				{
					//Change its pollfds.events
					this->pollfds[i].events = POLLOUT;
					//Get out of the loop
					break ;
				}
			}
		}
	//If state == WRITING, keep writing the response. Only close connection in case that there was a timeout or error.
	}
	if (this->clientConnections[client_fd]->isReadyToWrite())
	{
		if (!this->clientConnections[client_fd]->writeResponse(this->clientConnections[client_fd]->config.first))
		{
			closeClient(client_fd);
			return;
		}
		if (this->clientConnections[client_fd]->isResponseComplete())
		{
			//We erase the route
			if (this->clientConnections[client_fd]->response.headers.find("Connection")->second == "close")
			{
				closeClient(client_fd);
			}
			else 
			{
				// std::cout << this->clientConnections[client_fd]->response.headers.find("Connection")->second << std::endl;
				//After finishing the response, we change its state to listening again (if keep-alive) -> change state to POLLIN
				for (size_t i = 0; i < this->pollfds.size(); i++)
				{

					if (client_fd == this->pollfds[i].fd)
					{
						//RESET: revents to 0, events to POLLIN, request to READING
						//Change its pollfds.events
						this->pollfds[i].events = POLLIN;
						this->pollfds[i].revents = 0;
						this->clientConnections[client_fd]->cleanConnection();
						//Get out of the loop
						break ;
					}
				}
				//DEBUGGING:
				// std::cout << "state after request: " << std::endl;
				// std::cout << "fd: "  << this->pollfds[0].fd << " - events: " << this->pollfds[0].events << " - revents: " << this->pollfds[0].revents << std::endl;
				// std::cout << "fd: " <<this->pollfds[1].fd << " - " << " events: " << this->pollfds[1].events << " - revents: " << this->pollfds[1].revents << std::endl;

			}
		}
	}
}


void			ServerManager::closeClient(int client_fd)
{
	//Remove from pollfds vector
	for (size_t i = 0; i < this->pollfds.size(); i++)
	{
		if (client_fd == this->pollfds[i].fd)
		{
			//Erase it from pollfds
			this->pollfds.erase(this->pollfds.begin() + i);
			//Get out of the loop
			break ;
		}
	}
	for (std::map<int, ConnectionHandler*>::iterator it = this->clientConnections.begin(); it != this->clientConnections.end(); it++)
	{
		if (it->first == client_fd)
		{
			delete it->second;
			this->clientConnections.erase(it);
			break;
		}
	}
	//Close fd
	if (client_fd)
	{
		close(client_fd);
	}


}
