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

Log::~Log()
{
	
}
void Log::render() const {
		int width = texture->getFrameWidth();
		int height = texture->getFrameHeight();
		texture->render(SDL_FRect{ position.getX(), position.getY() ,(float)width,(float)height });
}
void Log::update() {
		position = position + speed;
		if (position.getX() > Game::WINDOW_WIDTH) {
			position = Point2D(-150.0f - (float)texture->getFrameWidth(), position.getY());
		}
}
Collision Log::checkCollision(const SDL_FRect& other) const {	
	SDL_FRect rect = { position.getX(), position.getY(), 
		(float)texture->getFrameWidth(), (float)texture->getFrameHeight() };

	if(SDL_HasRectIntersectionFloat(&rect, &other)) {
		return Collision(Collision::PLATFORM, speed);
	}
	return Collision(Collision::NONE);
	
}



