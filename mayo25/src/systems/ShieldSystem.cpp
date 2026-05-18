#include "ShieldSystem.h"

#include "../game/messages_defs.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include"../components/Immunity.h"
#include"../components/Miracle.h"

const float T = 10.0f;

void ShieldSystem::update() {

    auto currTime = sdlutils().virtualTimer().currTime() / 1000.0f;
    float elapsed = currTime - startTime;

    if (elapsed >= T) {
        deactivateShield();
    }
}

void ShieldSystem::recieve(const Message& m) {
    switch (m.id) {
    case _m_SHIELD_ACTIVATED:
        activateShield();
        break;
    case _m_ROUND_START:
        resetShield();
        break;
    }
}


void ShieldSystem::activateShield() {
    active = true;
    startTime = sdlutils().virtualTimer().currTime() / 1000.0f;

}

void ShieldSystem::deactivateShield() {
    active = false;

    Message m;
    m.id = _m_SHIELD_DEACTIVATED;
    _mngr->send(m);


}
void ShieldSystem::resetShield() {
    active = false;
    startTime = 0.0f;
    Message m;
    m.id = _m_SHIELD_DEACTIVATED;
    _mngr->send(m);
}