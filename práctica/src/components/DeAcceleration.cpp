#include "DeAcceleration.h"

DeAcceleration::DeAcceleration() :
	_tr(nullptr) {
}

DeAcceleration::~DeAcceleration() {
}

void DeAcceleration::initComponent() {
	_tr = _ent->getComponent<Transform>();
	assert(_tr != nullptr);
}

void DeAcceleration::update() {

	Vector2D& vel = _tr->getVel();
	vel = vel * 0.98f;

	if (vel.magnitude() < 0.05f) {
		vel = Vector2D(0.0f, 0.0f);
	}
}