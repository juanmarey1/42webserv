#pragma once

#include "webserv.hpp"

class RequestParser
{
	private:
		enum Phase {REQUEST_LINE, HEADERS, BODY, COMPLETE}	phase;		//Defines the stages of HTTTP request parsing
		HttpRequest											request;	//Store parsed data from request

		
	public:
		RequestParser();
		RequestParser(const RequestParser &original);
		RequestParser		&operator=(const RequestParser &original);
		~RequestParser();

		HttpRequest		getHttpRequest() const;							//Returns internal HttpRequest when isRequestComplete() == true
		bool			isRequestComplete() const;						//if phase==COMPLETE
		void			parseLine(const std::string &line);				//Depending on the current phase it will parse request line, parse a header, detect end of headers and start handling the body.

		class	InvalidRequestLineExcept: public std::exception
		{
			public:
				const char *what() const throw() {
					return ("Invalid request line!");
				}
		};

		class	InvalidHeadersExcept: public std::exception
		{
			public:
				const char *what() const throw() {
					return ("Invalid headers!");
				}
		};

};
