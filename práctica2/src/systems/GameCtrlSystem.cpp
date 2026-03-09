// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameCtrlSystem.h"

#include "../components/Points.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "StarsSystem.h"
GameCtrlSystem::GameCtrlSystem() :
		_score() {
	// TODO Auto-generated constructor stub

}

GameCtrlSystem::~GameCtrlSystem() {
	// TODO Auto-generated destructor stub
}

void GameCtrlSystem::initSystem() {
}

void GameCtrlSystem::update() {
	auto &ihldr = ih();

	if (ihldr.keyDownEvent()) {
		if (ihldr.isKeyDown(SDL_SCANCODE_SPACE)) {

			Message m;
			m.id = _m_CREATE_STARS;
			m.create_stars_data.n = 5;
			_mngr->send(m);
		}
	}
}

void GameCtrlSystem::recieve(const Message &m) {
	switch (m.id) {
	case _m_STAR_EATEN:
		_score += _mngr->getComponent<Points>(m.star_eaten_data.e)->_points;
		break;
	default:
		break;
	}
}
