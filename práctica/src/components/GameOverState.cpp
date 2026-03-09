#include "GameOverState.h"
#include "GameInfoMsgs.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../game/FighterUtils.h"
#include "../game/Game.h"

GameOverState::GameOverState(bool won) : _won(won) {}

void GameOverState::update()
{
    auto& inhdlr = ih();

    if (inhdlr.keyDownEvent()) {
        Game::Instance()->setState(Game::NEWGAME);
    }

    sdlutils().clearRenderer();

    auto& msg = sdlutils().msgs().at(_won ? "won" : "lost");

    msg.render(
        (sdlutils().width() - msg.width()) / 2,
        sdlutils().height() / 2 + msg.height() * 2
    );


    sdlutils().presentRenderer();
}

void GameOverState::enter()
{

}


