// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL_stdinc.h>

#include "../ecs/Component.h"

struct StarMotion: public ecs::Component {

	StarMotion() :
			_updateFreq(), _lastUpdate(), _rot(), _sizeLimit() {
	}

	virtual ~StarMotion() {
	}

	inline bool shouldUpdate(Uint32 currTime) {
		if (_lastUpdate + _updateFreq < currTime) {
			_lastUpdate = currTime;
			return true;
		} else {
			return false;
		}
	}

	Uint32 _updateFreq;
	Uint32 _lastUpdate;
	float _rot;
	float _sizeLimit;
};

