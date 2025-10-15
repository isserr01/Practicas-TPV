#include "Vehicle.h"

Vehicle::Vehicle(Game* game, const Point2D& position, const Vector2D<float>& speed,int type)
	: game(game)
	, texture(nullptr)
	,position(position)
	,speed(speed)
	, type(type)
{
	if (type == 1) texture = game->getTexture(Game::VEHICLE1);
	else if (type == 2) texture = game->getTexture(Game::VEHICLE2);
	else if (type == 3) texture = game->getTexture(Game::VEHICLE3);
	else if (type == 4) texture = game->getTexture(Game::VEHICLE4);
	else if (type == 5) texture = game->getTexture(Game::VEHICLE5);


}
void Vehicle::render() const {
	int width = texture->getFrameWidth();
	int height = texture->getFrameHeight();
	texture->render(SDL_FRect{ position.getX(), position.getY() ,(float)width,(float)height });
}
void Vehicle::update() {
	position = position + speed;
	if (position.getX() > Game::WINDOW_WIDTH) {
		position = Point2D(0 - (float)texture->getFrameWidth()/2, position.getY());
	}
	else if (position.getX() + texture->getFrameWidth() < -150) {
		position = Point2D(Game::WINDOW_WIDTH, position.getY());
	}
}

Collision Vehicle::checkCollision(const SDL_FRect& other) const {
	SDL_FRect rect = { position.getX(), position.getY(),
	(float)texture->getFrameWidth(), (float)texture->getFrameHeight() };

	if (SDL_HasRectIntersectionFloat(&rect, &other)) {
		return Collision(Collision::ENEMY);
	}
	return Collision(Collision::NONE);

}


