// This file is part of the course TPV2@UCM - Samir Genaim

#include "NewGameState.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "../game/Game.h"
#include "../components/GameInfoMsgs.h"
#include "../ecs/EntityManager.h"
#include "../components/FighterCtrl.h"
#include "../components/DeAcceleration.h"
#include "../components/Gun.h"
#include "../components/ShowAtOpposieSide.h"
#include "../components/Image.h"
#include "../game/FighterUtils.h"

NewGameState::NewGameState() :
	_msg(sdlutils().msgs().at("start")) {
	_fighterFacade = new FighterUtils();

}

NewGameState::~NewGameState() {
	delete _fighterFacade;
}

void NewGameState::leave() {
}

void NewGameState::update() {

	auto& inhdlr = ih();

	if (inhdlr.keyDownEvent()) {
		_fighterFacade->reset_lives();
		Game::Instance()->setState(Game::NEWROUND);
	}

	sdlutils().clearRenderer();


	_msg.render((sdlutils().width() - _msg.width()) / 2,
		sdlutils().height() / 2 + _msg.height() * 2);

	sdlutils().presentRenderer();

}

void NewGameState::enter() {
	_fighterFacade->reset_fighter();
}
