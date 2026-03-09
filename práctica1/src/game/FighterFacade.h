#pragma once
#include "../ecs/ecs.h"

class FighterFacade {
public:
	FighterFacade() {}
	virtual ~FighterFacade() {}

	virtual void create_fighter() = 0;
	virtual void reset_fighter() = 0;
	virtual void reset_lives() = 0;
	virtual int update_lives(int n) = 0;
};