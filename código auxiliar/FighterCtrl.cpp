#include "FighterCtrl.h"
#include "../sdlutils/InputHandler.h"
#include "Container.h"

FighterCtrl::FighterCtrl() :
	_right(SDL_SCANCODE_RIGHT), //
	_left(SDL_SCANCODE_LEFT), //
	_up(SDL_SCANCODE_UP)
{
}

FighterCtrl::~FighterCtrl() {
}

void FighterCtrl::handleInput(Container* o) {
	auto& ihdlr = ih();

	const float thrust = 0.2f;
	const float speedLimit = 3.0f;

	
		if (ihdlr.isKeyDown(_right)) {
			o->setRotation(o->getRotation() + 5.0f);
			std::cout << "Rotating right to " << o->getRotation() << std::endl;
		}
		else if (ihdlr.isKeyDown(_left)) {
			o->setRotation(o->getRotation() - 5.0f);
		}
		else if (ihdlr.isKeyDown(_up)) {
			Vector2D vel = o->getVel();
			float r = o->getRotation();

			Vector2D newVel = vel + Vector2D(0, -1).rotate(r) * thrust;
			if (newVel.magnitude() > speedLimit) {
				newVel = newVel.normalize() * speedLimit;
			}
			o->getVel().setY(newVel.getY());
			o->getVel().setX(newVel.getX());

		}
	
}
