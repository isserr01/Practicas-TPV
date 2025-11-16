#include "wasp.h"
#include "game.h"

wasp::wasp(Game* game, Texture* textura, Point2D posicion) :
	SceneObject(game)
	
{
	texture = game->getTexture(Game::WASP);
	this->position = posicion;
}
	
	 

void wasp:: render() const {
	if (alive) {
		int ancho = texture->getFrameWidth();
		int alto = texture->getFrameHeight();

		texture->render(SDL_FRect{ position.getX(),position.getY(),(float)ancho,(float)alto });
	}
	
}

void wasp::spawn() {

	durationAle=game->getRandomRange(50,100);
	
	alive = true;
	if (positionAle == 0) {
		position = { 20,WASP_SPAWN };
	}
	else if (positionAle == 1) {
		position = { 116,WASP_SPAWN };
	}
	else if (positionAle == 2) {
		position = { 212,WASP_SPAWN };
	}
	else if (positionAle == 3) {
		position = { 309,WASP_SPAWN };
	}
	else {
		position = { 405,25 };
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

Collision wasp::checkCollision(const SDL_FRect& other) {
	SDL_FRect rect =  getBoundingBox();
	if (SDL_HasRectIntersectionFloat(&rect, &other) && alive) {
		return Collision(Collision::ENEMY);
	}
	else {
		return Collision(Collision::NONE);
	}
}
bool wasp::isAlive() const{
	return alive;
}
int wasp::getPosition() const {
	return positionAle;
}
void wasp::SetPosition(const int& index) {
	positionAle = index;
}