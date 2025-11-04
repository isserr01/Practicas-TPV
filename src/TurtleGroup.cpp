#include "TurtleGroup.h"

TurtleGroup::TurtleGroup(Game* game, std::istream& in)
	: Platform(game, Vector2D<float>(0, 0), Point2D(0, 0))
{
	float x, y, s;
	int t, n;
	if (in >> n >>  y >> s >> t)
	{
		position = Point2D(Game::WINDOW_WIDTH, y);
		speed = Vector2D<float>(s, 0.0f);

		if (t == 1) isSubmersible = true;
		else if (t == 0) isSubmersible = false;

		numTurtles = n;

		texture = game->getTexture(Game::TURTLE);

		dimY = texture->getFrameHeight();
		dimX = texture->getFrameWidth();

	}

	frameNumber = 0;
	submergeTimer = 0.0f;
	submerged = false;
	submergePeriod = 2.0f;

}
void TurtleGroup::update() {
	
	Crosser::update();
	frameNumber = (frameNumber + 1 % 7);

	if (isSubmersible) {
		float deltaS = Game::FRAME_RATE * 0.001f;
		submergeTimer += deltaS;

		if (submergeTimer >= submergePeriod)
			submergeTimer = 0.0f;

		submerged = (frameNumber >= 4);
	}
	else submerged = false;

}

Collision TurtleGroup::checkCollision(const SDL_FRect& other) {
	if (isSubmersible && submerged)
		return Collision(Collision::NONE);

	return Platform::checkCollision(other);
}