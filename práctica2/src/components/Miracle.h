#pragma once

#include <SDL_stdinc.h>
#include "../sdlutils/SDLUtils.h"

#include "../ecs/Component.h"

struct Miracle : public ecs::Component {

	Miracle() : _isActive(false) {
		auto& rand = sdlutils().rand();
		N = rand.nextInt(10, 20);
		M = rand.nextInt(1, 5);
	}

	virtual ~Miracle() {
	}

	inline bool switchState(Uint32 currTime) {
		if (N + M < currTime && _isActive) {
			N = currTime;			
			return true;
			_isActive = false;
		}
		else {
			M = currTime;
			_isActive = true;
			return false;
		}
	}

	int N;
	int M;
	bool _isActive;

};
