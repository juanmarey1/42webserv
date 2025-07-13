#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <stdexcept>
#include <fstream>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> 
#include <cstring>
#include <utility>
#include <poll.h>
#include <ctime>

#ifndef BUFFERSIZE
# define BUFFERSIZE 4096
#endif


// #include "configFile.hpp"
// #include "server.hpp"
// #include "location.hpp"
// #include "token.hpp"
// #include "connection.hpp"

#include "ServerManager.hpp"
#include "ServerConfig.hpp"
#include "LocationConfig.hpp"
#include "ConnectionHandler.hpp"
#include "ConfigFileParser.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "RequestParser.hpp"
#include "ResponseBuilder.hpp"
#include "Router.hpp"
#include "ErrorHandler.hpp"
#include "Utils.hpp"
