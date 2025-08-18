// #include "../include/webserv.hpp"



// int	main()
// {
// 	ServerConfig	server;
// 	//Test 1: Valid CGI script execution
// 	CGIHandler	handler1;
// 	HttpRequest		request1;
// 	request1.method = "POST";
// 	request1.uri = "/cgi-bin/echo.py";
// 	request1.version = "HTTP/1.1";
// 	request1.body = "Hello, World!";
// 	request1.headers["Host"] = "localhost";
// 	request1.headers["Content-Length"] = "13";
// 	request1.headers["Content-Type"] = "text/plain";
// 	LocationConfig	location1;
// 	location1.locationPath = "/cgi-bin/";
// 	location1.allowed_methods.push_back("GET");
// 	location1.allowed_methods.push_back("POST");
// 	location1.cgi_extension = ".py";
// 	location1.cgi_path = "/home/jrey-roj/juanma/webserv/tests/var/www/cgi-bin/";
// 	location1.autoindex = false;
	
// 	std::cout << "\nTEST 1 .......................................... VALID CGI SCRIPT EXECUTION\n" << std::endl;
// 	HttpResponse	response = handler1.executeScript(request1, location1, server);

// 	std::cout << "\nHttp response:\n" << response.status_code << " " << response.status_text << "\n" << response.body << std::endl;
// 	for (std::map<std::string,  std::string>::iterator it = response.headers.begin(); it != response.headers.end(); it++) {
//         std::cout  << it->first << ": " << it->second << std::endl;
//     }

// 	std::string buffer = response.build();
// 	std::cout << "THE-BEGINNING\n" << buffer << "THE-END\n" << std::endl;
// 	std::cout << "\nSUCCESSS!!!!!!!!!!!!! :))))))))\n" << std::endl;


// 	//Test 2:
// 	CGIHandler	handler2;
// 	HttpRequest	request2;
// 	request2.method = "POST";
// 	request2.uri = "/cgi-bin/echo.py";
// 	request2.version = "HTTP/1.1";
// 	request2.body = "Hello, World!";
// 	request2.headers["Host"] = "localhost:8080";
// 	request2.headers["Content-Type"] = "text/plain";
// 	request2.headers["Content-Length"] = "13";
// 	request2.headers["User-Agent"] = "curl/7.88.1";
// 	request2.headers["Accept"] = "*/*"; 
// 	LocationConfig	location2;
// 	location2.locationPath = "/cgi-bin/";
// 	location2.cgi_extension = ".py";
// 	location2.cgi_path = "/home/jrey-roj/juanma/webserv/tests/var/www/cgi-bin/";
// 	std::cout << "\nTEST 2 .......................................... CORRECT ENVIRONMENT VARIABLES\n" << std::endl;
// 	HttpResponse	response2 = handler2.executeScript(request2, location2, server);

// 	std::string buffer2 = response2.build();
// 	std::cout << "THE-BEGINNING\n" << buffer2 << "THE-END\n" << std::endl;
// 	std::cout << "\nSUCCESSS!!!!!!!!!!!!! :))))))))\n" << std::endl;

// 	//Test 3: 
// 	CGIHandler	handler3;
// 	LocationConfig location3;
// 	location3.locationPath = "/cgi-bin/";
// 	location3.allowed_methods.push_back("GET");
// 	location3.cgi_extension = ".py";
// 	location3.cgi_path = "/home/jrey-roj/juanma/webserv/tests/var/www/cgi-bin/";
// 	HttpRequest request3;
// 	request3.method = "GET";
// 	request3.version = "HTTP/1.1";
// 	request3.uri = "/cgi-bin/echo2.py?name=Juan&age=19";
// 	request3.headers["Host"] = "localhost:8080";
// 	request3.headers["User-Agent"] = "curl/7.88.1";
// 	std::cout << "\nTEST 3 .......................................... SIMPLE GET REQUEST\n" << std::endl;
// 	HttpResponse	response3 = handler3.executeScript(request3, location3, server);

// 	std::cout << "\nHttp response:\n" << response3.status_code << " " << response3.status_text << "\n" << response3.body << std::endl;
// 	for (std::map<std::string,  std::string>::iterator it = response3.headers.begin(); it != response3.headers.end(); it++) {
//         std::cout  << it->first << ": " << it->second << std::endl;
//     }
// 	std::string buffer3 = response3.build();
// 	std::cout << "THE-BEGINNING\n" << buffer3 << "THE-END\n" << std::endl;
// 	std::cout << "\nSUCCESSS!!!!!!!!!!!!! :))))))))\n" << std::endl;

	
// 	//Test 4:
// 	CGIHandler	handler4;
// 	LocationConfig location4;
// 	location4.locationPath = "/cgi-bin/";
// 	location4.allowed_methods.push_back("GET");
// 	location4.allowed_methods.push_back("POST");
// 	location4.cgi_extension = ".py";
// 	location4.cgi_path = "/home/jrey-roj/juanma/webserv/tests/var/www/cgi-bin/";  // full absolute path preferred
// 	// location4.autoindex = false;
// 	HttpRequest	request4;
// 	request4.method = "POST";
// 	request4.version = "HTTP/1.1";
// 	request4.uri = "/cgi-bin/normie.py";
// 	request4.headers["Host"] = "localhost:8080";
// 	request4.headers["Content-Type"] = "application/x-www-form-urlencoded";
// 	request4.headers["Content-Length"] = "13";
// 	request4.body = "name=ChatGPT\n";
// 	std::cout << "\nTEST 4 .......................................... SIMPLE POST REQUEST\n" << std::endl;
// 	HttpResponse	response4 = handler4.executeScript(request4, location4, server);
	
// 	std::cout << "\nHttp response:\n" << response4.status_code << " " << response4.status_text << "\n" << response4.body << std::endl;
// 	for (std::map<std::string,  std::string>::iterator it = response4.headers.begin(); it != response4.headers.end(); it++) {
// 		std::cout  << it->first << ": " << it->second << std::endl;
//     }
// 	std::string buffer4 = response4.build();
// 	std::cout << "THE-BEGINNING\n" << buffer4 << "THE-END\n" << std::endl;
// 	std::cout << "\nSUCCESS!!!!!!!!!!!!! :))))))))\n" << std::endl;

// 	//test 5

// 	CGIHandler	handler5;
// 	LocationConfig	location5;
// 	location5.cgi_extension = ".py";
// 	location5.cgi_path = "/home/jrey-roj/juanma/webserv/tests/var/www/cgi-bin/";
// 	location5.allowed_methods.push_back("GET");
// 	location5.allowed_methods.push_back("POST");
// 	location5.root = "/home/jrey-roj/juanma/webserv/tests/var/www";
// 	location5.locationPath = "/cgi-bin/";

// 	HttpRequest	request5;
// 	request5.method = "POST";
// 	request5.uri = "/cgi-bin/echo4.py";
// 	request5.version =  "HTTP/1.1";
// 	request5.headers["Host"] = "localhost:8080";
// 	request5.headers["Content-Length"] = "0";

// 	std::cout << "\nTEST 5 .......................................... PROPER HEADERS RETURNED\n" << std::endl;
// 	HttpResponse	response5  = handler5.executeScript(request5, location5, server);
	
// 	std::cout << "\nHttp response:\n" << response5.status_code << " " << response5.status_text << "\n" << response5.body << std::endl;
// 	for (std::map<std::string,  std::string>::iterator it = response5.headers.begin(); it != response5.headers.end(); it++) {
// 		std::cout  << it->first << ": " << it->second << std::endl;
//     }
// 	std::string buffer5 = response5.build();
// 	std::cout << "THE-BEGINNING\n" << buffer5 << "THE-END\n" << std::endl;
// 	std::cout << "\nSUCCESS!!!!!!!!!!!!! :))))))))\n" << std::endl;


// 	//TEST 6
// 	CGIHandler	handler6;
// 	LocationConfig	location6;
// 	location6.locationPath = "/cgi-bin/";
// 	location6.cgi_path = "/home/jrey-roj/juanma/webserv/tests/var/www/cgi-bin/";
// 	HttpRequest	request6;
// 	request6.version = "HTTP/1.1";
// 	request6.uri = "/cgi-bin/notfound.py";
// 	request6.method = "POST";

// 	std::cout << "\nTEST 6 .......................................... CGI SCRIPT NOT FFOUND\n" << std::endl;
// 	HttpResponse	response6  = handler6.executeScript(request6, location6, server);
	
// 	std::cout << "\nHttp response:\n" << response6.status_code << " " << response6.status_text << "\n" << response6.body << std::endl;
// 	for (std::map<std::string,  std::string>::iterator it = response6.headers.begin(); it != response6.headers.end(); it++) {
// 		std::cout  << it->first << ": " << it->second << std::endl;
//     }
// 	std::string buffer6 = response6.build();
// 	std::cout << "THE-BEGINNING\n" << buffer6 << "THE-END\n" << std::endl;
// 	std::cout << "\nSUCCESS!!!!!!!!!!!!! :))))))))\n" << std::endl;

// 	//test 7
// 	CGIHandler	handler7;
// 	HttpRequest		request7;
// 	request7.method = "POST";
// 	request7.uri = "/cgi-bin/failExecute.py";
// 	request7.version = "HTTP/1.1";
// 	request7.body = "Hello, World!";
// 	request7.headers["Host"] = "localhost";
// 	request7.headers["Content-Length"] = "13";
// 	request7.headers["Content-Type"] = "text/plain";
// 	LocationConfig	location7;
// 	location7.locationPath = "/cgi-bin/";
// 	location7.allowed_methods.push_back("GET");
// 	location7.allowed_methods.push_back("POST");
// 	location7.cgi_extension = ".py";
// 	location7.cgi_path = "/home/jrey-roj/juanma/webserv/tests/var/www/cgi-bin/";
// 	location7.autoindex = false;

// 	std::cout << "\nTEST 7 .......................................... CGI SCRIPT NOT executabel\n" << std::endl;
// 	HttpResponse	response7  = handler7.executeScript(request7, location7, server);
	
// 	std::cout << "\nHttp response:\n" << response7.status_code << " " << response7.status_text << "\n" << response7.body << std::endl;
// 	for (std::map<std::string,  std::string>::iterator it = response7.headers.begin(); it != response7.headers.end(); it++) {
// 		std::cout  << it->first << ": " << it->second << std::endl;
//     }
// 	std::string buffer7 = response7.build();
// 	std::cout << "THE-BEGINNING\n" << buffer7 << "THE-END\n" << std::endl;
// 	std::cout << "\nSUCCESS!!!!!!!!!!!!! :))))))))\n" << std::endl;

// 	//TEST 8
// 	CGIHandler	handler8;
// 	HttpRequest		request8;
// 	request8.method = "POST";
// 	request8.uri = "/cgi-bin/error.py";
// 	request8.version = "HTTP/1.1";
// 	request8.body = "Hello, World!";
// 	request8.headers["Host"] = "localhost";
// 	request8.headers["Content-Length"] = "13";
// 	request8.headers["Content-Type"] = "text/plain";
// 	LocationConfig	location8;
// 	location8.locationPath = "/cgi-bin/";
// 	location8.allowed_methods.push_back("GET");
// 	location8.allowed_methods.push_back("POST");
// 	location8.cgi_extension = ".py";
// 	location8.cgi_path = "/home/jrey-roj/juanma/webserv/tests/var/www/cgi-bin/";
// 	location8.autoindex = false;
// 	std::cout << "\nTEST 8 .......................................... CGI malformed headers\n" << std::endl;
// 	HttpResponse	response8  = handler8.executeScript(request8, location8, server);
	
// 	std::cout << "\nHttp response:\n" << response8.status_code << " " << response8.status_text << "\n" << response8.body << std::endl;
// 	for (std::map<std::string,  std::string>::iterator it = response8.headers.begin(); it != response8.headers.end(); it++) {
// 		std::cout  << it->first << ": " << it->second << std::endl;
//     }
// 	std::string buffer8 = response8.build();
// 	std::cout << "THE-BEGINNING\n" << buffer8 << "THE-END\n" << std::endl;
// 	std::cout << "\nSUCCESS!!!!!!!!!!!!! :))))))))\n" << std::endl;

// 	//TEST 9
// 	CGIHandler	handler9;
// 	HttpRequest		request9;
// 	request9.method = "POST";
// 	request9.uri = "/cgi-bin/empty.py";
// 	request9.version = "HTTP/1.1";
// 	request9.body = "Hello, World!";
// 	request9.headers["Host"] = "localhost";
// 	request9.headers["Content-Length"] = "13";
// 	request9.headers["Content-Type"] = "text/plain";
// 	LocationConfig	location9;
// 	location9.locationPath = "/cgi-bin/";
// 	location9.allowed_methods.push_back("GET");
// 	location9.allowed_methods.push_back("POST");
// 	location9.cgi_extension = ".py";
// 	location9.cgi_path = "/home/jrey-roj/juanma/webserv/tests/var/www/cgi-bin/";
// 	location9.autoindex = false;
// 	std::cout << "\nTEST 9 .......................................... cgi WRITES NOTHING\n" << std::endl;
// 	HttpResponse	response9  = handler9.executeScript(request9, location9, server);
	
// 	std::cout << "\nHttp response:\n" << response9.status_code << " " << response9.status_text << "\n" << response9.body << std::endl;
// 	for (std::map<std::string,  std::string>::iterator it = response9.headers.begin(); it != response9.headers.end(); it++) {
// 		std::cout  << it->first << ": " << it->second << std::endl;
//     }
// 	std::string buffer9 = response9.build();
// 	std::cout << "THE-BEGINNING\n" << buffer9 << "THE-END\n" << std::endl;
// 	std::cout << "\nSUCCESS!!!!!!!!!!!!! :))))))))\n" << std::endl;

// 	CGIHandler	handler;
// 	HttpRequest		request;
// 	request.method = "POST";
// 	request.uri = "/cgi-bin/incorrectOrder.py";
// 	request.version = "HTTP/1.1";
// 	request.body = "Hello, World!";
// 	request.headers["Host"] = "localhost";
// 	request.headers["Content-Length"] = "13";
// 	request.headers["Content-Type"] = "text/plain";
// 	LocationConfig	location;
// 	location.locationPath = "/cgi-bin/";
// 	location.allowed_methods.push_back("GET");
// 	location.allowed_methods.push_back("POST");
// 	location.cgi_extension = ".py";
// 	location.cgi_path = "/home/jrey-roj/juanma/webserv/tests/var/www/cgi-bin/";
// 	location.autoindex = false;

// 	std::cout << "\nTEST 10 .......................................... incorrect order headers-body\n" << std::endl;
// 	HttpResponse response10;
// 	response10  = handler.executeScript(request, location, server);
// 	std::cout << "\nHttp  response:\nHTTP\\1.1 " << response10.status_code << " " << response10.status_text << std::endl;
// 	for (std::map<std::string,  std::string>::iterator it = response10.headers.begin(); it != response10.headers.end(); it++) {
// 		std::cout  << it->first << ": " << it->second << std::endl;
//     }
// 	std::cout  << std::endl;
// 	std::cout << response10.body << std::endl;
// 	std::string buffer10 = response10.build();
// 	std::cout << "THE-BEGINNING\n" << buffer10 << "THE-END\n" << std::endl;
// 	std::cout << "\nSUCCESS!!!!!!!!!!!!! :))))))))\n" << std::endl;

// 	//TEST 11
// 	CGIHandler		handler11;
// 	HttpRequest		request11;
// 	request11.method = "POST";
// 	request11.uri = "/cgi-bin/echo3.py?user=42";
// 	request11.version = "HTTP/1.1";
// 	request11.body = "CARLOS GAY";
// 	request11.headers["Host"] = "localhost";
// 	request11.headers["Content-Length"] = "13";
// 	request11.headers["Content-Type"] = "text/plain";
// 	request11.headers["Connection"] = "close";
	
// 	std::cout << "\nTEST 11 .......................................... query post\n" << std::endl;
// 	HttpResponse response11  = handler11.executeScript(request11, location, server);
// 	std::cout << "\nHttp  response:\nHTTP\\1.1 " << response11.status_code << " " << response11.status_text << std::endl;
// 	for (std::map<std::string,  std::string>::iterator it = response11.headers.begin(); it != response11.headers.end(); it++) {
// 		std::cout  << it->first << ": " << it->second << std::endl;
//     }
// 	std::cout  << std::endl;
// 	std::cout << response11.body << std::endl;
// 	std::string buffer11 = response11.build();
// 	std::cout << "THE-BEGINNING\n" << buffer11 << "THE-END\n" << std::endl;
// 	std::cout << "\nSUCCESS!!!!!!!!!!!!! :))))))))\n" << std::endl;

// 	//TEST 12
// 	CGIHandler	handler12;
// 	HttpRequest		request12;
// 	request12.method = "POST";
// 	request12.uri = "/cgi-bin/echo.py";
// 	request12.version = "HTTP/1.1";
// 	request12.body = "Hello, World!";
// 	request12.headers["Content-Length"] = "13";
// 	request12.headers["Content-Type"] = "text/plain";
// 	LocationConfig	location12;
// 	location12.locationPath = "/cgi-bin/";
// 	location12.allowed_methods.push_back("GET");
// 	location12.allowed_methods.push_back("POST");
// 	location12.cgi_extension = ".py";
// 	location12.cgi_path = "/home/jrey-roj/juanma/webserv/tests/var/www/cgi-bin/";
// 	location12.autoindex = false;
// 	std::cout << "\nTEST 12.......................................... no host\n" << std::endl;
// 	HttpResponse response12  = handler12.executeScript(request12, location, server);
// 	std::cout << "\nHttp  response:\nHTTP\\1.1 " << response12.status_code << " " << response12.status_text << std::endl;
// 	for (std::map<std::string,  std::string>::iterator it = response12.headers.begin(); it != response12.headers.end(); it++) {
// 		std::cout  << it->first << ": " << it->second << std::endl;
//     }
// 	std::cout  << std::endl;
// 	std::cout << response12.body << std::endl;
// 	std::string buffer12 = response12.build();
// 	std::cout << "THE-BEGINNING\n" << buffer12 << "THE-END\n" << std::endl;
// 	std::cout << "\nSUCCESS!!!!!!!!!!!!! :))))))))\n" << std::endl;

// 	//TEST 13
// 	CGIHandler	handler13;
// 	HttpRequest		request13;
// 	request13.method = "GET";
// 	request13.uri = "/cgi-bin/redirect.py";
// 	request13.version = "HTTP/1.1";
// 	request13.body = "Hello, World!";
// 	request13.headers["Content-Length"] = "13";
// 	request13.headers["Content-Type"] = "text/plain";
// 	request13.headers["Host"] = "localhost:8080";
// 	LocationConfig	location13;
// 	location13.locationPath = "/cgi-bin/";
// 	location13.allowed_methods.push_back("GET");
// 	location13.allowed_methods.push_back("POST");
// 	location13.cgi_extension = ".py";
// 	location13.cgi_path = "/home/jrey-roj/juanma/webserv/tests/var/www/cgi-bin/";
// 	location13.autoindex = false;
// 	std::cout << "\nTEST 13.......................................... REDIRECT\n" << std::endl;
// 	HttpResponse response13  = handler13.executeScript(request13, location13, server);
// 	std::cout << "\nHttp  response:\nHTTP/1.1 " << response13.status_code << " " << response13.status_text << std::endl;
// 	for (std::map<std::string,  std::string>::iterator it = response13.headers.begin(); it != response13.headers.end(); it++) {
// 		std::cout  << it->first << ": " << it->second << std::endl;
//     }
// 	std::cout  << std::endl;
// 	std::cout << response13.body << std::endl;
// 	std::string buffer13 = response13.build();
// 	std::cout << "THE-BEGINNING\n" << buffer13 << "THE-END\n"  << std::endl;
// 	std::cout << "\nSUCCESS!!!!!!!!!!!!! :))))))))\n" << std::endl;

	

// 	return (0);
// }
