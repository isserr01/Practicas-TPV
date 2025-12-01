#include "wasp.h"


wasp::wasp(GameState* state, Point2D posicion) :
	SceneObject(state)
	
{
	try {
		alive = false;
		durationAle = 0;
		position = posicion;
		texture = game->getTexture(Game::WASP);
		dimX = texture->getFrameWidth();
		dimY = texture->getFrameHeight();
		}
	catch (const std::string& e) {
		throw std::string("Error al cargar la avispa") + e;
	}
}
	

void wasp::spawn() {

	durationAle = game->getRandomRange(50,100);
	
	alive = true;
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
int wasp::getPosition() const {
	return positionAle;
}
void wasp::SetPosition(const int& index) {
	positionAle = index;

	static const Point2D positions[5] = {
		{20.0f, WASP_SPAWN},
		{116.0f, WASP_SPAWN},
		{212.0f, WASP_SPAWN},
		{309.0f, WASP_SPAWN},
		{405.0f, WASP_SPAWN}
	};
	position = positions[index];
}