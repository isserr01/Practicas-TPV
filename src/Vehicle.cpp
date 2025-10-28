#include "Vehicle.h"



Vehicle::Vehicle(Game* game, std::istream& in)
	: game(game),
	texture(nullptr)
{
	float x, y, s;
	int t;
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
	}


}

Vehicle::~Vehicle()
{
	
}

void Vehicle::render() const {
	int width = texture->getFrameWidth();
	int height = texture->getFrameHeight();
	texture->render(SDL_FRect{ position.getX(), position.getY() ,(float)width,(float)height });
}

void Vehicle::update() {
	//regla de tres
	position = position + speed * Game::FRAME_RATE * VEHICLE_SPEED_FACTOR;

	//para que los vehículos reaparezcan al salir de la pantalla
	if (position.getX() > Game::WINDOW_WIDTH) {
		position = Point2D(0 - (float)texture->getFrameWidth(), position.getY());
	}
	else if (position.getX() + texture->getFrameWidth() < -Game::SIDE_LIMIT) {
		position = Point2D(Game::WINDOW_WIDTH, position.getY());
	}
}

Collision Vehicle::checkCollision(const SDL_FRect& other) const {

	//rectángulo para comprobar colisión
	SDL_FRect rect = { position.getX(), position.getY(),
	(float)texture->getFrameWidth(), (float)texture->getFrameHeight() };

	//devuelve colisión con enemigo si hay intersección
	if (SDL_HasRectIntersectionFloat(&rect, &other)) {
		return Collision(Collision::ENEMY);
	}
	return Collision(Collision::NONE);

}


