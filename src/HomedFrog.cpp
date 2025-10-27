#include "HomedFrog.h"
#include "game.h"
HomedFrog::HomedFrog(Game* game, Texture* textura, Point2D posicion) : game(game), textura(textura), posicion(posicion) {}

void HomedFrog::render() const {
	int ancho = textura->getFrameWidth();
	int alto = textura->getFrameHeight();

	textura->renderFrame(SDL_FRect{ posicion.getX(), posicion.getY(),(float)ancho,(float)alto }, 0, 0);

}

