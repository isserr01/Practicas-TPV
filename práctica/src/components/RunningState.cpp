#include "RunningState.h"
#include "../game/Game.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "Health.h"

void RunningState::update() {
	auto* game = Game::Instance();
	auto* gm = game->getMngr();
	auto& ihdlr = ih();
	auto& vt = sdlutils().virtualTimer();
	auto ship = gm->getHandler(ecs::hdlr::FIGHTER);

	sdlutils().clearRenderer();

	gm->update();
	game->checkCollisions();
	gm->render();
	gm->refresh();
	if (vt.currTime() - _lastAsteroidTime >= 5000) {
		Game::Instance()->getAteroidsFacade()->create_asteroids(1);
		_lastAsteroidTime = vt.currTime();
	}

    if (ihdlr.keyDownEvent() && ihdlr.isKeyDown(SDL_SCANCODE_P)) {
        Game::Instance()->setState(Game::PAUSED);
    }

    Game::Instance()->getMngr()->render();

    sdlutils().presentRenderer();
}