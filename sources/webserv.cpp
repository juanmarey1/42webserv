#include "../include/webserv.hpp"

// int	main(int argc, char *argv[])
// {
// 	ConfigFile	configFile;

// 	if (argc > 2 || argc < 1) {
// 		std::cerr << "Invalid number of arguments" << std::endl;
// 	}
// 	try {
// 		configFile.openConfFile(argc, argv);
// 		configFile.parseConfFile();
// 		configFile.setUpServers();
// 	}
// 	catch (const std::exception &e) {
// 		std::cerr << e.what() << std::endl;
// 		exit (1);
// 	}

// 	return 0;
// }

// int		main(int argc, char *argv[])
// {
// 	ServerManager	serverManager;

// 	if (argc > 2 || argc < 1)
// 	{
// 		std::cerr << "Invalid number of arguments" << std::endl;
// 		exit (1);
// 	}
// 	try
// 	{
// 		if (argv[1])
// 		{
// 			serverManager.loadConfigParsing(argv[1]);
// 		}
// 		else
// 		{
// 			serverManager.loadConfigParsing("default.conf");
// 		}
// 		serverManager.initSockets();
// 		serverManager.run();
// 	}
// 	catch (const std::exception &e)
// 	{
// 		std::cerr << e.what() << std::endl;
// 		exit (1);
// 	}
// 	return (0);
// }
