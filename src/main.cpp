//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//

#include "game.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	try {
		Game* game = new Game();
		game->run();

		// TODO: manejar excepciones
		delete game;

	}
	catch (const string& e) {
		cerr << "Error: " << e << endl;
		return 1;
	}

	return 0;
	
}
