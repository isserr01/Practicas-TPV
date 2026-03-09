// This file is part of the course TPV2@UCM - Samir Genaim

#include "StarsSystem.h"

#include <algorithm>
#include "../components/Image.h"
#include "../components/Points.h"
#include "../components/StarMotion.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "GameCtrlSystem.h"

StarsSystem::StarsSystem() :
		_starsLimit(30), _currNumOfStars(0) {
}

StarsSystem::~StarsSystem() {
}

void StarsSystem::initSystem() {
}

void StarsSystem::update() {

	auto currTime = sdlutils().currRealTime();
	auto stars = _mngr->getEntities(ecs::grp::STARS);
	auto n = stars.size();

	for (auto i = 0u; i < n; i++) {
		auto tr = _mngr->getComponent<Transform>(stars[i]);
		auto starmotion = _mngr->getComponent<StarMotion>(stars[i]);

		if (starmotion->shouldUpdate(currTime)) {

			// rotate it
			tr->_rot += starmotion->_rot;

			// resize it
			tr->_width *= 0.95f;
			tr->_height *= 0.95f;

			// check if it should die
			if (tr->_width < starmotion->_sizeLimit
					|| tr->_height < starmotion->_sizeLimit) {
				_mngr->setAlive(stars[i], false);
				_currNumOfStars--;
			}
		}
	}
}

void StarsSystem::addStar(unsigned int n) {

	// Always use the random number generator provided by SDLUtils
	//
	auto &rand = sdlutils().rand();

	auto limit = std::min( //
			static_cast<unsigned int>(n), //
			_starsLimit - _currNumOfStars);

	for (auto i = 0u; i < limit; i++) {

		// add and entity to the manager
		//
		auto e = _mngr->addEntity(ecs::grp::STARS);

		// add a Transform component, and initialise it with random
		// size and position
		//
		auto tr = _mngr->addComponent<Transform>(e);
		auto s = rand.nextInt(50, 100);
		auto x = rand.nextInt(0, sdlutils().width() - s);
		auto y = rand.nextInt(0, sdlutils().height() - s);
		tr->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);

		// add an Image Component
		//
		_mngr->addComponent<Image>(e, &sdlutils().images().at("star"));

		// add a StarMotion component to resize/rotate the star
		//
		auto motion = _mngr->addComponent<StarMotion>(e);

		motion->_rot = rand.nextInt(5, 10);
		motion->_sizeLimit = rand.nextInt(2, 10);
		motion->_updateFreq = rand.nextInt(20, 100);

		auto pts = _mngr->addComponent<Points>(e);
		pts->_points = rand.nextInt(1, 5);
		_currNumOfStars++;
	}
}

void StarsSystem::onStarEaten(ecs::entity_t e) {
	_mngr->setAlive(e, false);
	_currNumOfStars--;

	// play sound on channel 1 (if there is something playing there
	// it will be cancelled
	sdlutils().soundEffects().at("pacman_eat").play("se");
}

void StarsSystem::recieve(const Message &m) {
	switch (m.id) {
	case _m_STAR_EATEN:
		onStarEaten(m.star_eaten_data.e);
		break;
	case _m_CREATE_STARS:
		addStar(m.create_stars_data.n);
		break;
	default:
		break;
	}
}
