#pragma once

#include "PhysicsComponent.h"

class Container;

class ShowAtOppositeSide : public PhysicsComponent
{

public:
	ShowAtOppositeSide();
	virtual ~ShowAtOppositeSide();
	void update(Container* o) override;


private:
	float top;
	float bottom;
	float left;
	float right;

};

