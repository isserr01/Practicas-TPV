//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//

#include "game.h"
#include <iostream>
#include <string>
#include "GameError.h"

using namespace std;

int main(int argc, char* argv[])
{
	try {
		Game game;
		game.run();

	}
	catch (const GameError& e) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Error en el juego",
			e.what(),
			nullptr);
		return 1;
	}
	

	return 0;
	
}
