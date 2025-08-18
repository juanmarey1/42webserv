#pragma once

#include "webserv.hpp"

class ConnectionHandler
{
	public:

		ConnectionHandler();
		ConnectionHandler(int fd);
		ConnectionHandler(const ConnectionHandler &original);
		ConnectionHandler	&operator=(const ConnectionHandler &original);
		~ConnectionHandler();

		HttpRequest									request;				//Incoming HTTP requst from a client
		HttpResponse								response;				//HTTP response to send back to the client
		std::pair<ServerConfig, LocationConfig>		config;					//The exact server and location
		std::time_t									lastActivity;	//Timestamp of the last operation (to implement timeouts)

		bool				readRequest();			//Called when poll() says socket is readable. Uses recv(), appends to internal buffer and calls parseRequest who checks if full request is received. 
		bool				parseRequest();			//Parses the HttpRequest and if it is complete, it changes its state to WRITING and stores the HttpRequest
		bool 				isRequestComplete();	//if state != READING
		bool				isReadyToWrite();		//if state == WRITING
		bool				writeResponse(const ServerConfig &server);		//Sends HTTP response data back to the client on the socket fd. use it when poll() signals socket is ready for writing and state==WRITING. We may have to send on chunks
		bool				isResponseComplete();	//If state == READY
		void				cleanConnection();

	private:
		int 									fd;      		//client socket FD. to write, read or send data  
		std::string								buffer;			//Temporary storage of incoming/outcoming data
		enum State {READING, WRITING, READY}	state;			//Tracks the phase the client connection is in
		std::string								sendBuffer;		//The buffer needed to send (because it may come in chunks)
		size_t 									sendOffset;		//sendBuffer buffersize

};
