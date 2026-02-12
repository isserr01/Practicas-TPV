#include "ShowAtOppositeSide.h"
#include "Container.h"


ShowAtOppositeSide::ShowAtOppositeSide() : 
	top(600.0f),
	bottom(0.0f), 
	left(0.0f), 
	right(800.0f)
{
}

ShowAtOppositeSide::~ShowAtOppositeSide() {
}

void ShowAtOppositeSide::update(Container* o) {
	if (o->getPos().getY() + o->getHeight() < bottom) {
		o->getPos().setY(top);
	}
	else if (o->getPos().getY() > top) {
		o->getPos().setY(bottom - o->getHeight());
	}
	else if(o->getPos().getX() + o->getWidth() < left) {
		o->getPos().setX(right);
	}
	else if(o->getPos().getX() > right) {
		o->getPos().setX(left - o->getWidth());
	}
}