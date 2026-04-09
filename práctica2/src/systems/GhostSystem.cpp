// This file is part of the course TPV2@UCM - Samir Genaim

#include "GhostSystem.h"

#include "../components/Image.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/messages_defs.h"

#include <cstdlib>

GhostSystem::GhostSystem() :
	_pmTR(nullptr),timerGhost(0.0f),immunity(false), cantidadGhosts(0) {
}

GhostSystem::~GhostSystem() {
}

void GhostSystem::spawnGhost() {
	// create the Ghost entity
	auto ghost = _mngr->addEntity();
	_mngr->setHandler(ecs::grp::GHOST, ghost);

	auto s = 50.0f;
	auto x = sdlutils().width();
	auto y = sdlutils().height();
	Vector2D pos;
	int r = rand() % 4;
	switch (r)
	{
	case 0: pos = Vector2D(0, 0); break;
	case 1: pos = Vector2D(x - s, 0); break;
	case 2: pos = Vector2D(0, y - s); break;
	case 3: pos = Vector2D(x - s, y - s); break;
	}

	_gmTR = _mngr->addComponent<Transform>(ghost);
	_gmTR->init(pos, Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(pacman, &sdlutils().images().at("pacman"));
	ghosts.push_back(ghost);
	velocities[ghost] = randomVelocity();
}

void GhostSystem::update(float dt) {
	if (!immunity && timerGhost >= 5.0f && ghosts.size() < 10) {
		spawnGhost();
		timerGhost = 0.0f;
	}

	auto pacman = _mngr->getHandler(ecs::hdlr::PACMAN);
	auto pmTR = _mngr->getComponent<Transform>(pacman);
	Vector2D pmPos = pmTR->pos;

	for (auto g : ghosts) {
		auto& tr = g->getComponent<Transform>();
		auto v = velocities[g];
		tr.pos += v * dt;

		float r = rand() / RAND_MAX;

		if(r<0.005f){
			Vector2D dir = (pmPos - tr.pos).normalize();
			v = dir * 1.1f;
		}
		float x = tr.pos.getX();
		float y = tr.pos.getY();
		float w = sdlutils().width();
		float h = sdlutils().height();

		if (x <= 0 || x >= w)
			v.setX(-v.getX());

		if (y <= 0 || y >= h)
			v.setY(-v.getY());
	}
}
Vector2D GhostSystem::randomVelocity() {

	float vx = (rand() % 3 - 1);
	float vy = (rand() % 3 - 1);

	Vector2D v(vx, vy);

	if (v.magnitude() == 0)
		v = Vector2D(1, 0);

	return v.normalize() * 1.1f;
}
