#include "HomedFrog.h"
#include "game.h"

HomedFrog::HomedFrog(Game* game, Point2D posicion)
	: SceneObject(game)
{
	try {
		position = posicion;
		texture = game->getTexture(Game::FROG);
		dimX = texture->getFrameWidth();
		dimY = texture->getFrameHeight();
	}
	catch (const std::string& e) {
		throw std::string("Error al cargar la rana desde el mapa") + e;
	}
}


void HomedFrog::render() const  {

	texture->renderFrame(SDL_FRect{ position.getX(), position.getY(),(float)dimX,(float)dimY }, 0, 0);

}
void HomedFrog::update() {
	}

HomedFrog::~HomedFrog() {}



