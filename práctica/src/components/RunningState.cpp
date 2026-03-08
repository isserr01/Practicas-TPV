#include "RunningState.h"
#include "../game/Game.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

void RunningState::update() {
	auto* game = Game::Instance();
	auto* gm = game->getMngr();

	sdlutils().clearRenderer();

	gm->update();
	//gm->checkCollisions();
	gm->render();
	gm->refresh();

    auto& inhdlr = ih();

    if (inhdlr.keyDownEvent() && inhdlr.isKeyDown(SDL_SCANCODE_P)) {
        Game::Instance()->setState(Game::PAUSED);
    }

    Game::Instance()->getMngr()->render();

    sdlutils().presentRenderer();
}