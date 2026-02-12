#pragma once

#include "PhysicsComponent.h"

class Container;

class DeAcceleration : public PhysicsComponent
{

public:
		DeAcceleration();
		virtual ~DeAcceleration();
		void update(Container* o) override;


private:
	float decelFactor;

	

};

