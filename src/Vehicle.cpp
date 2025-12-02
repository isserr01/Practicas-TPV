#include "Vehicle.h"



Vehicle::Vehicle(GameState* state, std::istream& in)
	: Crosser(state,Vector2D<float>(0,0),Point2D(0,0))
{
	float x, y, s;
	int t;
	Game* game = state->getGame();
	try {
		if (in >> x >> y >> s >> t)
		{
			position = Point2D(x, y);
			speed = Vector2D<float>(s, 0.0f);

			//selecciona la textura según el tipo
			if (t == 1) texture = game->getTexture(Game::VEHICLE1);
			else if (t == 2) texture = game->getTexture(Game::VEHICLE2);
			else if (t == 3) texture = game->getTexture(Game::VEHICLE3);
			else if (t == 4) texture = game->getTexture(Game::VEHICLE4);
			else if (t == 5) texture = game->getTexture(Game::VEHICLE5);

			dimY = texture->getFrameHeight();
			dimX = texture->getFrameWidth();

		}
	}
	catch (const std::string& e) {
		throw std::string("Error al cargar los vehículos desde el mapa") + e;
	}

}

Collision Vehicle::checkCollision(const SDL_FRect& other) {

	//rectángulo para comprobar colisión
	SDL_FRect rect = getBoundingBox();

	//devuelve colisión con enemigo si hay intersección
	if (SDL_HasRectIntersectionFloat(&rect, &other)) {
		return Collision(Collision::ENEMY);
	}
	return Collision(Collision::NONE);

}


