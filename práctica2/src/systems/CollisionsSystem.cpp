// This file is part of the course TPV2@UCM - Samir Genaim

#include "CollisionsSystem.h"

#include "../components/Transform.h"
#include "../components/Health.h"
#include "../ecs/EntityManager.h"
#include "../utils/Collisions.h"
#include "StarsSystem.h"

CollisionsSystem::CollisionsSystem() : _isImmune(false) {}

CollisionsSystem::~CollisionsSystem() {}

void CollisionsSystem::initSystem() {}

void CollisionsSystem::recieve(const Message& m) {
    switch (m.id) {
    case _m_IMMUNITY_START:
        _isImmune = true;
        break;
    case _m_IMMUNITY_END:
        _isImmune = false;
        break;
    default:
        break;
    }
}

void CollisionsSystem::update() {
    auto pm = _mngr->getHandler(ecs::hdlr::PACMAN);
    auto pTR = _mngr->getComponent<Transform>(pm);

    // Colisiones con estrellas
    auto& stars = _mngr->getEntities(ecs::grp::STARS);
    auto n = stars.size();
    for (auto i = 0u; i < n; i++) {
        auto e = stars[i];
        if (_mngr->isAlive(e)) {
            auto eTR = _mngr->getComponent<Transform>(e);
            if (Collisions::collides(pTR->_pos, pTR->_width, pTR->_height,
                eTR->_pos, eTR->_width, eTR->_height)) {
                Message m;
                m.id = _m_STAR_EATEN;
                m.star_eaten_data.e = e;
                _mngr->send(m);
            }
        }
    }

    // Colisiones con fantasmas
    auto& ghosts = _mngr->getEntities(ecs::grp::GHOSTS);
    for (auto i = 0u; i < ghosts.size(); i++) {
        auto e = ghosts[i];
        if (_mngr->isAlive(e)) {
            auto gTR = _mngr->getComponent<Transform>(e);
            if (Collisions::collides(pTR->_pos, pTR->_width, pTR->_height,
                gTR->_pos, gTR->_width, gTR->_height)) {

                Message m;
                if (_isImmune) {
                    // Pacman es inmune - el fantasma muere
                    m.id = _m_GHOST_EATEN;
                   
                }
                else {
                    // Pacman no es inmune - Pacman recibe daño
                    m.id = _m_PACMAN_GHOST_COLLISION;
                   
                }
                m.ghost_coll_data.e = e;
                _mngr->send(m);
            }
        }
    }
}

