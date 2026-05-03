#include "ImmunitySystem.h"
#include "../game/messages_defs.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include"../components/Immunity.h"
#include"../components/Miracle.h"

void ImmunitySystem::update() {
    if (!active) return;
    float currTime = sdlutils().currRealTime();
    float elapsed = currTime - startTime;

    if (elapsed >= 10000.0f) { 
        deactivateImmunity();
    }
}

void ImmunitySystem:: recieve(const Message& m) {
    switch (m.id) {
        case _m_PACMAN_FOOD_COLLISION:
            onFruitEaten(m.fruit_eaten_data.e);
            break;
        case _m_ROUND_START:
            resetImmunity();
			break;
    }
}

void ImmunitySystem::onFruitEaten(ecs::entity_t fruit) {
   auto miracle = _mngr->getComponent<Miracle>(fruit);

    if (miracle!=nullptr && miracle->isMiraculous()) {
        activateImmunity();
    }
}

void ImmunitySystem::activateImmunity() {
   if(active) return; // Si ya está activa, no hacer nada
    active = true;
    startTime = sdlutils().currRealTime();
    
	Message m;
    m.id = _m_IMMUNITY_START;
    _mngr->send(m);
    
 
}

void ImmunitySystem::deactivateImmunity() {
    active = false;

	Message m;
	m.id = _m_IMMUNITY_END;
	_mngr->send(m);

 
}
void ImmunitySystem::resetImmunity() {
    active = false;
    startTime = 0.0f;
	Message m;
	m.id = _m_IMMUNITY_END;
	_mngr->send(m);
}