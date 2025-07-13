#include "../includes/webserv.hpp"

int	main(int argc, char *argv[])
{
	ConfigFile	configFile;

	if (argc > 2 || argc < 1) {
		std::cerr << "Invalid number of arguments" << std::endl;
	}
	try {
		configFile.openConfFile(argc, argv);
		configFile.parseConfFile();
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		exit (1);
	}

	return 0;
}
