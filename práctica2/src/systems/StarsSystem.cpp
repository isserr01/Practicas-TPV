// This file is part of the course TPV2@UCM - Samir Genaim

#include "StarsSystem.h"

#include <algorithm>
#include "../components/Image.h"
#include "../components/Points.h"
#include "../components/StarMotion.h"
#include "../components/Miracle.h"
#include "../components/Transform.h"
#include "../components/ImageWithFrames.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "GameCtrlSystem.h"
#include "../game/Game.h"

StarsSystem::StarsSystem() :
		_starsLimit(48), _currNumOfStars(0), _starPosX(10), _starPosY(10) {
}

StarsSystem::~StarsSystem() {
}

void StarsSystem::initSystem() {

}

void StarsSystem::update() {
	auto currTime = sdlutils().currRealTime() / 1000.0f;
	auto stars = _mngr->getEntities(ecs::grp::STARS);

	for (auto e : stars) {
		auto miracle = _mngr->getComponent<Miracle>(e);

		// Si no tiene componente Miracle, es una estrella normal y nunca cambia
		if (miracle == nullptr) {
			continue;
		}

		auto anim = _mngr->getComponent<ImageWithFrames>(e);

		if (miracle->switchState(currTime)) {
			if (miracle->isMiraculous()) {
				anim->setFrames({ 12 });  // Frame para estado milagroso
			}
			else {
				anim->setFrames({ 15 });  // Frame para estado normal
			}
		}
	}
}

void StarsSystem::addStar(unsigned int n) {

	// Always use the random number generator provided by SDLUtils
	//
	auto &rand = sdlutils().rand();

	for (auto i = 0u; i < 6; i++) {
		for (auto j = 0u;j < 8;j++) {

			int m = rand.nextInt(1, 10);
			// add and entity to the manager
			//
			auto e = _mngr->addEntity(ecs::grp::STARS);

			// add a Transform component, and initialise it with random
			// size and position
			//
			auto tr = _mngr->addComponent<Transform>(e);
			tr->init(Vector2D(_starPosX, _starPosY), Vector2D(), 30, 30, 0.0f);

			// add an Image Component
			//
			auto tex = &sdlutils().images().at("sprites");
			auto anim =_mngr->addComponent<ImageWithFrames>(e,tex,8,8,100);
			anim->setFrames({ 12 });

			// add a StarMotion component to resize/rotate the star
			//
			auto motion = _mngr->addComponent<StarMotion>(e);
			if (m == 1) _mngr->addComponent<Miracle>(e);

			auto pts = _mngr->addComponent<Points>(e);
			pts->_points = rand.nextInt(1, 5);

			_starPosX += 100;
			
			_currNumOfStars++;
		}
		_starPosY += 100;
		_starPosX = 0;
	}
}

void StarsSystem::onStarEaten(ecs::entity_t e) {
	_mngr->setAlive(e, false);
	_currNumOfStars--;

	if(_currNumOfStars == 0) {
		Message m;
		m.id = _m_ROUND_OVER;
		Game::Instance()->getMngr()->send(m);
		Game::Instance()->setState(Game::GAMEOVER_WON);
	}

	// play sound on channel 1 (if there is something playing there
	// it will be cancelled
	sdlutils().soundEffects().at("pacman_eat").play("se");
}
void StarsSystem::resetTimers() {
	auto stars = _mngr->getEntities(ecs::grp::STARS);
	Uint32 now = sdlutils().currRealTime();

	for (auto e : stars) {
		auto miracle = _mngr->getComponent<Miracle>(e);
		if (miracle != nullptr) {  // Solo resetear las milagrosas
			miracle->reset(now);
		}
	}
}

void StarsSystem::recieve(const Message &m) {
	switch (m.id) {
	case _m_STAR_EATEN:
		onStarEaten(m.star_eaten_data.e);
		break;
	case _m_CREATE_STARS:
		addStar(m.create_stars_data.n);
		break;
	case _m_ROUND_START:
		resetTimers();
		break;
	default:
		break;
	}
}
