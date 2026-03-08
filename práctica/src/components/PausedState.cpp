#include "PausedState.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../game/Game.h"
#include <iostream>

void PausedState::leave() {
	sdlutils().virtualTimer().resume();
}

void PausedState::enter() {
	sdlutils().virtualTimer().pause();
	std::cout << "paused state" << std::endl;
}

void PausedState::update() {
	if (ih().keyDownEvent()) {
		Game::Instance()->setState(Game::RUNNING);
	}

	auto& msg = sdlutils().msgs().at("continue");

	msg.render(
		(sdlutils().width() - msg.width()) / 2,
		sdlutils().height() / 2 + msg.height() * 2
	);

	sdlutils().presentRenderer();
}
