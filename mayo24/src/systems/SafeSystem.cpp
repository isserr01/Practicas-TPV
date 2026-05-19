#include "SafeSystem.h"


#include "../game/messages_defs.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include"../components/Immunity.h"
#include"../components/Miracle.h"

void SafeSystem::update() {

    auto currTime = sdlutils().virtualTimer().currTime() / 1000.0f;
    float elapsed = currTime - startTime;

	std::cout << "SafeSystem update: elapsed time = " << elapsed << " seconds" << std::endl;

    if (elapsed >= 10.0f) {
        deactivateSafe();
    }
}

void SafeSystem::recieve(const Message& m) {
    switch (m.id) {
    case _m_SAFE_ACTIVATED:
        activateSafe();
        break;
    case _m_ROUND_START:
        resetSafe();
        break;
    }
}


void SafeSystem::activateSafe() {
    active = true;
    startTime = sdlutils().virtualTimer().currTime() / 1000.0f;

	std::cout << "Safe mode activated!" << std::endl;

}

void SafeSystem::deactivateSafe() {
    active = false;

    Message m;
    m.id = _m_SAFE_DEACTIVATED;
    _mngr->send(m);


}
void SafeSystem::resetSafe() {
    active = false;
    startTime = 0.0f;
    Message m;
    m.id = _m_SAFE_DEACTIVATED;
    _mngr->send(m);
}