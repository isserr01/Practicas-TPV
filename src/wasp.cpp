#include "wasp.h"
#include "game.h"

wasp::wasp(Game* game, Texture* textura, Point2D posicion) : game(game), textura(textura), posicion(posicion){}

void wasp:: render() const {
	if (alive) {
		int ancho = textura->getFrameWidth();
		int alto = textura->getFrameHeight();

		textura->render(SDL_FRect{ posicion.getX(),posicion.getY(),(float)ancho,(float)alto });
	}
	
}

void wasp::spawn() {

	durationAle=game->getRandomRange(50,100);
	positionAle= game->getRandomRange(0,4);
	alive = true;
	if (positionAle == 0) {
		posicion = { 20,WASP_SPAWN };
	}
	else if (positionAle == 1) {
		posicion = { 116,WASP_SPAWN };
	}
	else if (positionAle == 2) {
		posicion = { 212,WASP_SPAWN };
	}
	else if (positionAle == 3) {
		posicion = { 309,WASP_SPAWN };
	}
	else {
		posicion = { 405,25 };
	}
}
void wasp::update() {
	if (durationAle > 0) {
		durationAle--;
		if (durationAle == 0) {
			alive = false;
		}
	}
}

Collision wasp::checkCollision(const SDL_FRect& other) const {
	SDL_FRect rect = { posicion.getX(),posicion.getY(),(float)textura->getFrameWidth(),(float)textura->getFrameHeight() };
	if (SDL_HasRectIntersectionFloat(&rect, &other)) {
		return Collision(Collision::ENEMY);
	}
	else {
		return Collision(Collision::NONE);
	}
}
bool wasp::isAlive() const{
	return alive;
}