#include "DeAcceleration.h"
#include "Container.h"	

DeAcceleration::DeAcceleration() :
	decelFactor(0.995f) 
{
}

DeAcceleration::~DeAcceleration() {
}

void DeAcceleration::update(Container* o) {
	o->getVel().setX(o->getVel().getX() * decelFactor);
	o->getVel().setY(o->getVel().getY() * decelFactor);
}
