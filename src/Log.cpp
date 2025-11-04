#include "Log.h"


Log::Log(Game* game, std::istream& in)
	: Platform(game, Vector2D<float>(0,0), Point2D(0,0))
{

	float x, y, s;
	int t;

	if (in >> x >> y >> s >> t)
	{
		position = Point2D(x, y);
		speed = Vector2D<float>(s, 0.0f);

		//selecciona la textura según el tipo
		if (t == 0) this->texture = game->getTexture(Game::LOG1);
		else if (t == 1) this->texture = game->getTexture(Game::LOG2);

		dimX = texture->getFrameWidth();
		dimY = texture->getFrameHeight();
	}

}






