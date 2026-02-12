#include "NewRoundState.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "../game/Game.h"
#include "../components/GameInfoMsgs.h"
#include "../ecs/EntityManager.h"


void NewRoundState::update() {

    auto& inhdlr = ih();

    if (inhdlr.keyDownEvent() && inhdlr.isKeyDown(SDL_SCANCODE_RETURN)) {
        Game::Instance()->setState(Game::RUNNING);
    }

    sdlutils().clearRenderer();

    auto& msg = sdlutils().msgs().at("newround");

    msg.render(
        (sdlutils().width() - msg.width()) / 2,
        sdlutils().height() / 2 + msg.height() * 2
    );

    sdlutils().presentRenderer();
}


