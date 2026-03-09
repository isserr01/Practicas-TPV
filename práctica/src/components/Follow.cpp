#include "Follow.h"

Follow::Follow() :
	_tr(nullptr)
{
}
Follow::~Follow() {
}

void Follow::initComponent() {
	_tr = _ent->getComponent<Transform>();
	assert(_tr != nullptr);
}

void Follow::update() {

	auto fighter = Game::Instance()->getMngr()
		->getHandler(ecs::hdlr::FIGHTER);

	if (fighter == nullptr)
		return;

	auto fighterTr = fighter->getComponent<Transform>();

	Vector2D& vel = _tr->getVel();
	Vector2D pos = _tr->getPos();
	Vector2D target = fighterTr->getPos();

	vel = vel.rotate(vel.angle(target - pos) > 0 ? 1.0f : -1.0f);
}