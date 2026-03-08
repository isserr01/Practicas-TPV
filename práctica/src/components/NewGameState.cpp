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

NewGameState::NewGameState() :
	_msg(sdlutils().msgs().at("start")) {

}

NewGameState::~NewGameState() {
	// TODO Auto-generated destructor stub
}

void NewGameState::leave() {
}

void NewGameState::update() {

	auto& inhdlr = ih();

	if (inhdlr.keyDownEvent() && inhdlr.isKeyDown(SDL_SCANCODE_SPACE)) {
		Game::Instance()->setState(Game::NEWROUND);
	}

	sdlutils().clearRenderer();


	_msg.render((sdlutils().width() - _msg.width()) / 2,
		sdlutils().height() / 2 + _msg.height() * 2);

	sdlutils().presentRenderer();

}

void NewGameState::enter() {
	/*auto mngr = Game::Instance()->getMngr();

	int w = 50;
	int h = 50;

	Vector2D pos(
		(sdlutils().width() - w) / 2.0f,
		(sdlutils().height() - h) / 2.0f
	);

	auto fighter = mngr->addEntity();

	fighter->addComponent<Transform>(
		pos,
		Vector2D(0, 0),
		w,
		h,
		0.0f
	);

	fighter->addComponent<Image>(
		&sdlutils().images().at("fighter")
	);

	fighter->addComponent<FighterCtrl>()->initComponent();
	fighter->addComponent<DeAcceleration>();
	fighter->addComponent<Gun>();
	fighter->addComponent<ShowAtOpposieSide>();*/
}
