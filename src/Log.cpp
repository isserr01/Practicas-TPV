#include "Log.h"

Log::Log(Game* game, const Point2D& position, const Vector2D<float>& speed, int type)
	: game(game)
	, texture(nullptr)
	, position(position)
	, speed(speed)
{
	if (type == 0) texture = game->getTexture(Game::LOG1);
	else if (type == 1) texture = game->getTexture(Game::LOG2);

}
void Log::render() const {
		int width = texture->getFrameWidth();
		int height = texture->getFrameHeight();
		texture->render(SDL_FRect{ position.getX(), position.getY() ,(float)width,(float)height });
}
void Log::update() {
		position = position + speed;
		if (position.getX() > Game::WINDOW_WIDTH) {
			position = Point2D(-150.0f, position.getY());
		}
}
Collision Log::checkCollision(const SDL_FRect& other) const {
	SDL_FRect mine = { position.getX(), position.getY(), 
		(float)texture->getFrameWidth(), (float)texture->getFrameHeight() };

	bool collisionX = mine.x < other.x
		+ other.w && mine.x + mine.w > other.x;
	bool collisionY = mine.y < other.y
		+ other.h && mine.y + mine.h > other.y;

	if(collisionX && collisionY) {
		return Collision(CollisionType::LOG);
	}
	else {
		return Collision(CollisionType::NONE);
	}
}



