// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../components/GameInfoMsgs.h"
#include "../components/GameState.h"
#include "../components/Image.h"
#include "../components/Transform.h"
#include "../ecs/Entity.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"
#include "../game/AsteroidsUtils.h"
#include "../game/FighterUtils.h"


using ecs::EntityManager;

Game::Game() :
		_mngr(nullptr), //
		_state(nullptr) {
}


Game::~Game() {
	delete _mngr;

	// release InputHandler if the instance was created correctly.
	if (InputHandler::HasInstance())
		InputHandler::Release();

	// release SLDUtil if the instance was created correctly.
	if (SDLUtils::HasInstance())
		SDLUtils::Release();
}

bool Game::init() {

	if (!SDLUtils::Init("Asteroids", 800, 600,
		"resources/config/asteroids.resources.json"))
		return false;

	if (!InputHandler::Init()) {
		std::cerr << SDL_GetError() << std::endl;
		return false;
	}
	_fighterFacade = new FighterUtils();
	_asteroidsFacade = new AsteroidsUtils();


	return true;
}


void Game::start() {

	auto &ihdlr = ih();
	auto& vt = sdlutils().virtualTimer();
	// reset the time before starting - so we calculate correct
	// delta-time in the first iteration
	//
	vt.resetTime();
	// a boolean to exit the loop
	bool exit = false;

	while (!exit) {
		// store the current time -- all game objects should use this time when
		// then need to the current time. They also have accessed to the time elapsed
		// between the last two calls to regCurrTime().
		Uint32 startTime = vt.regCurrTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		assert(_state != nullptr);
		_state->update();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}

}

void Game::checkCollisions() {
	


}
