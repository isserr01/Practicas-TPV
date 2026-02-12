// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameInfoMsgs.h"

#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"
#include "GameState.h"
#include "../game/Game.h"
#include "../sdlutils/Texture.h"

GameInfoMsgs::GameInfoMsgs() :
		_gameState(nullptr) {

}

GameInfoMsgs::~GameInfoMsgs() {
	// TODO Auto-generated destructor stub
}

void GameInfoMsgs::initComponent() {

}

void GameInfoMsgs::render() {

	auto state = Game::Instance()->getCurrentState();

	// message when game is not running
	if (state != Game::RUNNING) {

		// game over message
		if (state == Game::GAMEOVER) {
			auto &t = sdlutils().msgs().at("gameover");
			t.render((sdlutils().width() - t.width()) / 2,
					(sdlutils().height() - t.height()) / 2);
		}

		// new game message
		if (state == Game::NEWGAME) {
			auto &t = sdlutils().msgs().at("newgame");
			t.render((sdlutils().width() - t.width()) / 2,
					sdlutils().height() / 2 + t.height() * 2);
		}
		else if (state == Game::NEWROUND) {
			auto& t = sdlutils().msgs().at("newround");
			t.render((sdlutils().width() - t.width()) / 2,
				sdlutils().height() / 2 + t.height() * 2);
		}
	}

	// score
	

}
