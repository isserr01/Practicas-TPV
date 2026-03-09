#include "MaterialConsistency.h"

MaterialConsistency::MaterialConsistency(int consistency) :
	_consistency(consistency),
	_lastCheck(0)
{
}

MaterialConsistency::~MaterialConsistency() {
}

void MaterialConsistency::update() {

	auto& vt = sdlutils().virtualTimer();

	if (vt.currTime() > _lastCheck + 5000) {

		_lastCheck = vt.currTime();

		int prob = sdlutils().rand().nextInt(0, 10);

		if (prob == 0) {  // 10% probabilidad
			_consistency--;

			if (_consistency <= 0)
				_ent->setAlive(false);
		}
	}
}