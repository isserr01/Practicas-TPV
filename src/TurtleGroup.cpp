#include "TurtleGroup.h"

TurtleGroup::TurtleGroup(Game* game, std::istream& in)
	: Platform(game, Vector2D<float>(0, 0), Point2D(0, 0))
{
	float x, y, s;
	int t, n;
	if (in >> x >> y >> s >> n >> t)
	{
		position = Point2D(x, y);
		speed = Vector2D<float>(s, 0.0f);

		

		numTurtles = n;
		isSubmersible = (t == 1);

		texture = game->getTexture(Game::TURTLE);

		dimY = texture->getFrameHeight();
		dimX = texture->getFrameWidth() * numTurtles;

	}

	frameNumber = 0;
	submergeTimer = 7.0f;
	frameDuration = 1.0f;
	submerged = false;
	submergePeriod = 0.0f;

}
void TurtleGroup::update() {
	
	Crosser::update();
	
	float delta = Game::FRAME_RATE * 0.01f;
	submergeTimer += delta;

	if (isSubmersible) {
		if (submergeTimer >= frameDuration) {
			frameNumber = (frameNumber + 1) % 7;
			submergeTimer = 0.0f;
		}
		submerged = (frameNumber >= 4);
	}
		
	else submerged = false;

}

void TurtleGroup::render() const {

	if (submerged) return;

	for (int i = 0; i < numTurtles; ++i) {
		SDL_FRect dest{ position.getX() + i*texture->getFrameWidth(),
		position.getY(),
		(float)texture->getFrameWidth(),
		(float)texture->getFrameHeight() };

		texture->renderFrame(dest, 0, frameNumber);
	}
	
	
}

Collision TurtleGroup::checkCollision(const SDL_FRect& other) {
	if (isSubmersible && submerged)
		return Collision(Collision::NONE);
	return Platform::checkCollision(other);
}