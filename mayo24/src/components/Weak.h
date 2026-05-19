#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"
#include <cassert>
#include "../utils/Vector2D.h"

class Weak : public ecs::Component
{
	int N;
	Vector2D _pos;
public:
	Weak(Vector2D pos) : N(3), _pos(pos) {};
	virtual ~Weak() {};
	void subCrashes() {
		N--;
	}

	int getCrashes() const {
		return N;
	}

	Vector2D getPos() const {
		return _pos;
	}


};


