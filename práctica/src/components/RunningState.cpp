#include "RunningState.h"
#include "../game/Game.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

void RunningState::update() {
	auto* game = Game::Instance();
	auto* gm = game->getMngr();

	gm->update();
	//gm->checkCollisions();
	gm->render();
	gm->refresh();



    auto& inhdlr = ih();

    if (inhdlr.keyDownEvent() && inhdlr.isKeyDown(SDL_SCANCODE_P)) {
        Game::Instance()->setState(Game::PAUSED);
    }

    sdlutils().clearRenderer();

    //auto& msg = sdlutils().msgs().at("newround");
    /*
    msg.render(
        (sdlutils().width() - msg.width()) / 2,
        sdlutils().height() / 2 + msg.height() * 2
    );*/

    sdlutils().presentRenderer();
    


}