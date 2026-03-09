// This file is part of the course TPV2@UCM - Samir Genaim

#include "CollisionsSystem.h"

#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../utils/Collisions.h"
#include "StarsSystem.h"

CollisionsSystem::CollisionsSystem() {
	// TODO Auto-generated constructor stub

}

CollisionsSystem::~CollisionsSystem() {
	// TODO Auto-generated destructor stub
}

void CollisionsSystem::initSystem() {
}

void CollisionsSystem::update() {

	// the PacMan's Transform
	//
	auto pm = _mngr->getHandler(ecs::hdlr::PACMAN);
	auto pTR = _mngr->getComponent<Transform>(pm);

	// For safety, we traverse with a normal loop until the current size. In this
	// particular case we could use a for-each loop since the list stars is not
	// modified.
	//
	auto &stars = _mngr->getEntities(ecs::grp::STARS);
	auto n = stars.size();
	for (auto i = 0u; i < n; i++) {
		auto e = stars[i];
		if (_mngr->isAlive(e)) { // if the star is active (it might have died in this frame)

			// the Star's Transform
			//
			auto eTR = _mngr->getComponent<Transform>(e);

			// check if PacMan collides with the Star (i.e., eat it)
			if (Collisions::collides(			//
					pTR->_pos, pTR->_width, pTR->_height, //
					eTR->_pos, eTR->_width, eTR->_height)) {

				Message m;
				m.id = _m_STAR_EATEN;
				m.star_eaten_data.e = e;
				_mngr->send(m);

			}
		}
	}

}

