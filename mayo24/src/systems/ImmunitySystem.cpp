#include "ImmunitySystem.h"
#include "../game/messages_defs.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include"../components/Immunity.h"
#include"../components/Miracle.h"

void ImmunitySystem::update() {

	auto currTime = sdlutils().virtualTimer().currTime() / 1000.0f; 
    float elapsed = currTime - startTime;

    if (elapsed >= 10.0f) { 
        deactivateImmunity();
    }
}

void ImmunitySystem:: recieve(const Message& m) {
    switch (m.id) {
	    case _m_IMMUNITY_START:
		    activateImmunity();
            break;
        case _m_ROUND_START:
            resetImmunity();
			break;
    }
}


void ImmunitySystem::activateImmunity() {
    active = true;
	startTime = sdlutils().virtualTimer().currTime() / 1000.0f; 
 
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