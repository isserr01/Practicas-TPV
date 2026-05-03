#include "RunningState.h"
#include "../game/Game.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/GameCtrlSystem.h"
#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"

void RunningState::update() {
	auto* game = Game::Instance();
	//auto* gm = game->getMngr();
	auto& ihdlr = ih();
	auto& vt = sdlutils().virtualTimer();

	sdlutils().clearRenderer();

	if (ihdlr.keyDownEvent() && ihdlr.isKeyDown(SDL_SCANCODE_P)) {
		//Game::Instance()->setState(Game::PAUSED);
	}

	for (auto sys : systems) {
		sys->update();
	}

	sdlutils().presentRenderer();
}

void RunningState::enter()
{

	

}

void RunningState::receive(const Message& m)
{
	switch (m.id) {
	case _m_ROUND_OVER:
		//Game::Instance()->setState(Game::PAUSED);
		break;
	default:
		break;
	}
}

