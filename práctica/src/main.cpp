// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>

#include "game/Game.h"



void start() {
	
	try {

		std::cout << "1\n";

		if (!Game::Init()) {
			std::cerr << "Init failed\n";
			return;
		}

		std::cout << "2\n";

		Game::Instance()->initGame();

		std::cout << "3\n";

		Game::Instance()->start();

		std::cout << "4\n";

	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unknown Error\n";
	}

	
}

int main(int, char**) {

	try {
		start();
	}
	catch (const std::string& e) { // catch exceptions thrown as strings
		std::cerr << e << std::endl;
	}
	catch (const char* e) { // catch exceptions thrown as char*
		std::cerr << e << std::endl;
	}
	catch (const std::exception& e) { // catch exceptions thrown as a sub-type of std::exception
		std::cerr << e.what();
	}
	catch (...) {
		std::cerr << "Caught and exception of unknown type ...";
	}

	return 0;
}

