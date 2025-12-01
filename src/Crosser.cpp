#include "Crosser.h"



Crosser::Crosser(GameState* state, Vector2D<float> s, Point2D position)
	: SceneObject(state),
		speed(s)
{
	this->position = position;
}

void Crosser::update() {
	//regla de tres 
	position = position + speed * Game::FRAME_RATE * 0.001;

	//para que los troncos reaparezcan al salir de la pantalla
	if (position.getX() > Game::WINDOW_WIDTH + Game::SIDE_LIMIT) {
		position = Point2D(-Game::SIDE_LIMIT, position.getY());
	}
	else if (position.getX() + texture->getFrameWidth() < -Game::SIDE_LIMIT) {
		position = Point2D(Game::WINDOW_WIDTH, position.getY());
	}


}
