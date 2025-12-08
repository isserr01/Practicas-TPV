#include "wasp.h"


wasp::wasp(GameState* state, Point2D posicion) :
	SceneObject(state), game(state->getGame())
	
{
	alive = false;
	durationAle = 0;
	position = posicion;
	texture = game->getTexture(Game::WASP);
	dimX = texture->getFrameWidth();
	dimY = texture->getFrameHeight();
			
}

wasp::wasp(GameState* state, std::istream& in)
	: SceneObject(state), game(state->getGame()),fromMap(true)
{

	float x, y, vx, vy;
	int t;

	try {
		if (in >> x >> y >> vx >> vy >> t)
		{
			position = Point2D(x, y);
			speed = Vector2D<float>(vx, vy);
			alive = true;
			durationAle = t;

			texture = game->getTexture(Game::WASP);

			dimX = texture->getFrameWidth();
			dimY = texture->getFrameHeight();
		}

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


	if (fromMap) {
		position = position + speed * Game::FRAME_RATE * 0.01;

		if (durationAle > 0) {
			durationAle--;
			if (durationAle == 0) {
				alive = false;
			}
		}
	}
	else {
		if (durationAle > 0) {
			durationAle--;
			if (durationAle == 0) {
				alive = false;
			}
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
void wasp::SetPosition(int index) {
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

