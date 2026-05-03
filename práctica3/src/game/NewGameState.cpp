// This file is part of the course TPV2@UCM - Samir Genaim

#include "NewGameState.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "../game/Game.h"
#include "../ecs/EntityManager.h"

NewGameState::NewGameState() :
	_msg(sdlutils().msgs().at("newgame")) {
	

}

NewGameState::~NewGameState() {}

void NewGameState::leave() {
}

void NewGameState::update() {
	 
	auto& inhdlr = ih();

	if (inhdlr.keyDownEvent()) {
		Message m;
		m.id = _m_NEW_GAME;
		//Game::Instance()->getMngr()->send(m);
		//Game::Instance()->setState(Game::NEWROUND);
	}

	sdlutils().clearRenderer();

	_msg.render((sdlutils().width() - _msg.width()) / 2,
		sdlutils().height() / 2 + _msg.height() * 2);

	sdlutils().presentRenderer();

}

void NewGameState::enter() {
	sdlutils().soundEffects().at("pacman_intro").play("se");
}
