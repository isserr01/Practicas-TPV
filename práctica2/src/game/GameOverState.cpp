#include "GameOverState.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../game/Game.h"

GameOverState::GameOverState(bool won) : _won(won) {}

void GameOverState::update()
{
	auto& inhdlr = ih();

	if (inhdlr.keyDownEvent()) {
		Game::Instance()->setState(Game::NEWGAME);
	}

	sdlutils().clearRenderer();

	const Texture& msg = _won ? sdlutils().msgs().at("won") : sdlutils().msgs().at("gameover");
	msg.render(
		(sdlutils().width() - msg.width()) / 2,
		sdlutils().height() / 2 + msg.height() * 2
	);


	sdlutils().presentRenderer();
}

void GameOverState::enter()
{

}


