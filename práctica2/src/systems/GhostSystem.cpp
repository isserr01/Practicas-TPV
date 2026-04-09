/*// This file is part of the course TPV2@UCM - Samir Genaim

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
	_mngr->addComponent<Image>(ghost, &sdlutils().images().at("pacman"));
	ghosts.push_back(ghost);
}

void GhostSystem::update(float dt) {
	if (!immunity && timerGhost >= 5.0f && cantidadGhosts < 10) {
		spawnGhost();
		timerGhost = 0.0f;
		cantidadGhosts++;
	}
	
	for (auto g : ghosts) {
		auto& tr = g->getComponent<Transform>();

		float r = rand() / RAND_MAX;
		if (r < 0.005f) {
			auto pacPos= (pmPos - tr.pos).normalize();
			tr->_vel= pacPos * 1.1f;
		}
		tr->_pos = tr->_pos + tr->_vel;


		//q rebote en las paredes
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
*/

// This file is part of the course TPV2@UCM - Samir Genaim

#include "GhostSystem.h"

#include "../components/Image.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

GhostSystem::GhostSystem() :
	_pmTR(nullptr) {
}

GhostSystem::~GhostSystem() {
}

void GhostSystem::initSystem() {
	// create the PacMan entity
	//
	auto pacman = _mngr->addEntity();
	_mngr->setHandler(ecs::hdlr::PACMAN, pacman);

	_pmTR = _mngr->addComponent<Transform>(pacman);
	auto s = 50.0f;
	auto x = (sdlutils().width() - s) / 2.0f;
	auto y = (sdlutils().height() - s) / 2.0f;
	_pmTR->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(pacman, &sdlutils().images().at("pacman"));
}

void GhostSystem::update() {
	/*
	auto& ihldr = ih();

	if (ihldr.keyDownEvent()) {

		if (ihldr.isKeyDown(SDL_SCANCODE_RIGHT)) { // rotate right
			_pmTR->_rot = _pmTR->_rot + 90.0f;

			// also rotate the PacMan so it looks in the same
			// direction where it moves
			//
			_pmTR->_vel = _pmTR->_vel.rotate(90.0f);
		}
		else if (ihldr.isKeyDown(SDL_SCANCODE_LEFT)) { // rotate left
			_pmTR->_rot = _pmTR->_rot - 90.0f;

			// also rotate the PacMan so it looks in the same
			// direction where it moves
			//
			_pmTR->_vel = _pmTR->_vel.rotate(-90.0f);
		}
		else if (ihldr.isKeyDown(SDL_SCANCODE_UP)) { // increase speed

			// add 1.0f to the speed (respecting the limit 3.0f). Recall
			// that speed is the length of the velocity vector
			float speed = std::min(3.0f, _pmTR->_vel.magnitude() + 1.0f);

			// change the length of velocity vecto to 'speed'. We need
			// '.rotate(rot)' for the case in which the current speed is
			// 0, so we rotate it to the same direction where the PacMan
			// is looking
			//
			_pmTR->_vel = Vector2D(0, -speed).rotate(_pmTR->_rot);
		}
		else if (ihldr.isKeyDown(SDL_SCANCODE_DOWN)) { // decrease speed
			// subtract 1.0f to the speed (respecting the limit 0.0f). Recall
			// that speed is the length of the velocity vector
			float speed = std::max(0.0f, _pmTR->_vel.magnitude() - 1.0f);

			// change the length of velocity vector to 'speed'. We need
			// '.rotate(rot)' for the case in which the current speed is
			// 0, so we rotate it to the same direction where the PacMan
			// is looking
			//
			_pmTR->_vel = Vector2D(0, -speed).rotate(_pmTR->_rot);
		}

	}

	// move the pacman
	_pmTR->_pos = _pmTR->_pos + _pmTR->_vel;

	// check left/right borders
	if (_pmTR->_pos.getX() < 0) {
		_pmTR->_pos.setX(0.0f);
		_pmTR->_vel.set(0.0f, 0.0f);
	}
	else if (_pmTR->_pos.getX() + _pmTR->_width > sdlutils().width()) {
		_pmTR->_pos.setX(sdlutils().width() - _pmTR->_width);
		_pmTR->_vel.set(0.0f, 0.0f);
	}

	// check upper/lower borders
	if (_pmTR->_pos.getY() < 0) {
		_pmTR->_pos.setY(0.0f);
		_pmTR->_vel.set(0.0f, 0.0f);
	}
	else if (_pmTR->_pos.getY() + _pmTR->_height > sdlutils().height()) {
		_pmTR->_pos.setY(sdlutils().height() - _pmTR->_height);
		_pmTR->_vel.set(0.0f, 0.0f);
	}
	*/
}
