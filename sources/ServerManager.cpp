#include "../include/webserv.hpp"

//Constructors, operators and destructor

ServerManager::ServerManager() {}

ServerManager::ServerManager(const ServerManager &original)
{
	this->serversList = original.serversList;
	this->listeningSockets = original.listeningSockets;
	this->clientConnections = original.clientConnections;
	this->pollfds = original.pollfds;
}

ServerManager	&ServerManager::operator=(const ServerManager &original)
{
	if (this != &original)
	{
		this->serversList = original.serversList;
		this->listeningSockets = original.listeningSockets;
		this->clientConnections = original.clientConnections;
		this->pollfds = original.pollfds;
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
			bind(sockfd, (sockaddr*)&addr, sizeof(addr));
			//We listen for incoming connections
			listen(sockfd, SOMAXCONN);

			//We store the socket FD and the related config in a map
			this->listeningSockets[sockfd] = this->serversList[i];

			//We add pollfd struct to pollfds vector with the event flag POLLIN (waits for a readable envent)
			pollfd	pfd;
			pfd.fd = sockfd;
			pfd.events = POLLIN;
			this->pollfds.push_back(pfd);
		}
	}
}


void			ServerManager::run()
{
	//Runs the main loop of poll()
	while (true)
	{
		int	timeout = 0;
		//Returns the number of ready sockets, or 0 on timeout, or -1 on error
		int	ready = poll(this->pollfds.data(), this->pollfds.size(), timeout);

		if (ready <= 0)
		{
			continue;
		}
		for (size_t i = 0; i < this->pollfds.size(); i++)
		{
			//If no events skip
			if (this->pollfds[i].revents == 0)
			{
				continue;
			}

			int fd = this->pollfds[i].fd;

			//check if socket has data to read
			if (this->pollfds[i].revents & POLLIN)
			{
				//If it is a server socket (found in the map) -> New client trying to connect. acceptClient(fd) to accept connection and add new client
				if (this->listeningSockets.count(fd))
				{
					acceptNewClient(fd);
				}
				//If it is a client socket (not found in the map) -> Call handleClient to read data, parse requests, etc.
				else 
				{
					handleClientRequest(fd);
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
	int	client_fd = accept(server_fd, NULL, NULL);
	if (client_fd < 0)
	{
		return;
	}
	//Set the new socket to non-blocking mode
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	//Creates a new ConnectionHandler object to track client state. Store in clients map
	this->clientConnections[client_fd] = new ConnectionHandler(client_fd);
	//Add client socket to pollfds vector and marks it for reading
	pollfd	fds;
	fds.fd = client_fd;
	fds.revents = POLLIN;
	this->pollfds.push_back(fds);
}


void			ServerManager::handleClientRequest(int client_fd)
{
	//If state == READING, keep reading the request. Only close connection in case that there was a timeout or error
	if (!this->clientConnections[client_fd]->isRequestComplete())
	{
		if (!this->clientConnections[client_fd]->readRequest())
		{
			closeClient(client_fd);
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
			
			ip = this->listeningSockets[client_fd].ip;
			port = this->listeningSockets[client_fd].port;
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
		}
		if (this->clientConnections[client_fd]->isResponseComplete())
		{
			//We erase the route

			//After finishing the response, we change its state to listening again (if keep-alive) -> change state to POLLIN
			for (size_t i = 0; i < this->pollfds.size(); i++)
			{
				if (client_fd == this->pollfds[i].fd)
				{
					//Change its pollfds.events
					this->pollfds[i].events = POLLIN;
					//Get out of the loop
					break ;
				}
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
	//Close fd
	if (client_fd)
	{
		close (client_fd);
	}

	std::map<int, ConnectionHandler*>::iterator it = this->clientConnections.find(client_fd);
	if (it != this->clientConnections.end())
	{
		//Delete object from heap
		delete it->second;
		//Erase fd from map
		this->clientConnections.erase(it);
	}

}
