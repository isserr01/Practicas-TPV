//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//

#include "game.h"

int main(int argc, char* argv[])
{
	Game* game = new Game();
	game->run();

	// TODO: manejar excepciones
	delete game;
	
	return 0;
	
}
