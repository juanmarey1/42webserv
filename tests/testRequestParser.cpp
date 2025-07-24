// #include "../include/webserv.hpp"

// int	main(void)
// {

// 	//TEST 1
// 	RequestParser		parser;
// 	std::string			line;
// 	size_t				pos = 0;
// 	std::string buffer = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";

// 	while (Utils::getNextLine(buffer, pos, line))
// 	{
	
// 		parser.parseLine(line);
// 	}
// 	std::cout << "TEST 1........................VALID GET REQUEST" << std::endl;
// 	std::cout <<"Is HTTPRequest complete: " << parser.isRequestComplete() << std::endl; 
// 	HttpRequest	request = parser.getHttpRequest();
// 	std::cout << request.method	<< " " << request.uri << " " << request.version << std::endl;
// 	for (std::map<std::string,  std::string>::iterator it = request.headers.begin(); it != request.headers.end(); it++) {
//         std::cout  << it->first << ": " << it->second << std::endl;
//     }
// 	std::cout << "\r\n"<< request.body << std::endl;
// 	std::cout << "STATE: " << parser.isRequestComplete() << std::endl;
// 	std::cout << "ERROR: " << request.error << std::endl;
// 	std::cout << "SUCCESSSS!!!!!!!!!!!!!                         \n" << std::endl;

// 	//TEST 2
// 	RequestParser		parser2;
// 	std::string			line2;
// 	size_t				pos2 = 0;
// 	std::string buffer2 = "POST /submit HTTP/1.1\r\nHost: localhost\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 13\r\n\r\ndata=value123";

// 	while (Utils::getNextLine(buffer2, pos2, line2))
// 	{
	
// 		parser2.parseLine(line2);
// 	}
// 	std::cout << "TEST 2........................VALID POST REQUEST" << std::endl;
// 	std::cout <<"Is HTTPRequest complete: " << parser2.isRequestComplete() << std::endl; 
// 	HttpRequest	request2 = parser2.getHttpRequest();
// 	std::cout << request2.method	<< " " << request2.uri << " " << request2.version << std::endl;
// 	for (std::map<std::string,  std::string>::iterator it = request2.headers.begin(); it != request2.headers.end(); it++) {
//         std::cout  << it->first << ": " << it->second << std::endl;
//     }
// 	std::cout << "\r\n"<< request2.body << std::endl;
// 	std::cout << "STATE: " << parser2.isRequestComplete() << std::endl;
// 	std::cout << "ERROR: " << request2.error << std::endl;
// 	std::cout << "SUCCESSSS!!!!!!!!!!!!!                         \n" << std::endl;

// 	//TEST 3
// 	RequestParser		parser3;
// 	std::string			line3;
// 	size_t				pos3 = 0;
// 	std::string buffer3 = "GET /page?user=42 HTTP/1.1\r\nHost: localhost\r\n\r\n";

// 	while (Utils::getNextLine(buffer3, pos3, line3))
// 	{
	
// 		parser3.parseLine(line3);
// 	}
// 	std::cout << "TEST 3........................VALID GET QUERY STRING" << std::endl;
// 	std::cout <<"Is HTTPRequest complete: " << parser3.isRequestComplete() << std::endl; 
// 	HttpRequest	request3 = parser3.getHttpRequest();
// 	std::cout << request3.method	<< " " << request3.uri << " " << request3.version << std::endl;
// 	for (std::map<std::string,  std::string>::iterator it = request3.headers.begin(); it != request3.headers.end(); it++) {
//         std::cout  << it->first << ": " << it->second << std::endl;
//     }
// 	std::cout << "\r\n"<< request3.body << std::endl;
// 	std::cout << "STATE: " << parser3.isRequestComplete() << std::endl;
// 	std::cout << "ERROR: " << request3.error << std::endl;
// 	std::cout << "SUCCESSSS!!!!!!!!!!!!!                         \n" << std::endl;


// 	//TEST4
// 	RequestParser		parser4;
// 	std::string			line4;
// 	size_t				pos4 = 0;
// 	std::string buffer4 = "GET / HTTP/1.1\r\nHost: localhost\r\nUser-Agent: TestClient\r\nAccept: */*\r\n\r\n";

// 	while (Utils::getNextLine(buffer4, pos4, line4))
// 	{
	
// 		parser4.parseLine(line4);
// 	}
// 	std::cout << "TEST 4........................GET MULTIPLE HEADERS REQUEST" << std::endl;
// 	std::cout <<"Is HTTPRequest complete: " << parser4.isRequestComplete() << std::endl; 
// 	HttpRequest	request4 = parser4.getHttpRequest();
// 	std::cout << request4.method	<< " " << request4.uri << " " << request4.version << std::endl;
// 	for (std::map<std::string,  std::string>::iterator it = request4.headers.begin(); it != request4.headers.end(); it++) {
//         std::cout  << it->first << ": " << it->second << std::endl;
//     }
// 	std::cout << "\r\n"<< request4.body << std::endl;
// 	std::cout << "STATE: " << parser4.isRequestComplete() << std::endl;
// 	std::cout << "ERROR: " << request4.error << std::endl;
// 	std::cout << "SUCCESSSS!!!!!!!!!!!!!                         \n" << std::endl;


// 	//TEST 5
// 	RequestParser		parse5;
// 	std::string			line5;
// 	size_t				pos5 = 0;
// 	std::string buffer5 = "POST / HTTP/1.1\r\nhost: fdsa\r\nUSER-agent: test\r\n connection : keep-alive \r\n\r\njuanmafdsa";

// 	while (Utils::getNextLine(buffer5, pos5, line5))
// 	{
	
// 		parse5.parseLine(line5);
// 	}
// 	std::cout << "TEST 5........................" << std::endl;
// 	std::cout <<"Is HTTPRequest complete: " << parse5.isRequestComplete() << std::endl; 
// 	HttpRequest	request5 = parse5.getHttpRequest();
// 	std::cout << request5.method	<< " " << request5.uri << " " << request5.version << std::endl;
// 	for (std::map<std::string,  std::string>::iterator it = request5.headers.begin(); it != request5.headers.end(); it++) {
//         std::cout  << it->first << ": " << it->second << std::endl;
//     }
// 	std::cout << "\r\n"<< request5.body << std::endl;
// 	std::string		buffer6 = "junnnnnnnn";
// 	pos5 = 0;
// 	while (Utils::getNextLine(buffer6, pos5, line5))
// 	{
	
// 		parse5.parseLine(line5);
// 	}
// 	request5 = parse5.getHttpRequest();
// 	std::cout << request5.body << std::endl;
// 	std::cout << "STATE: " << parse5.isRequestComplete() << std::endl;
// 	std::cout << "ERROR: " << request5.error << std::endl;
// 	std::cout << "SUCCESSSS!!!!!!!!!!!!!                         \n" << std::endl;

// 	return (0);
// }