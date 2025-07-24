#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
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
#include <sys/stat.h>
#include <sstream>
#include <iomanip>
#include <sys/wait.h>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <dirent.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 4096
#endif


// #include "configFile.hpp"
// #include "server.hpp"
// #include "location.hpp"
// #include "token.hpp"
// #include "connection.hpp"

#include "LocationConfig.hpp"
#include "ServerConfig.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "ConnectionHandler.hpp"
#include "ServerManager.hpp"
#include "ResponseBuilder.hpp"
#include "ConfigFileParser.hpp"
#include "RequestParser.hpp"
#include "Router.hpp"
#include "ErrorHandler.hpp"
#include "Utils.hpp"
#include "CGIHandler.hpp"
