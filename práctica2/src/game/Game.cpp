// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/GameCtrlSystem.h"
#include "../systems/PacManSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/StarsSystem.h"
#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"

using ecs::EntityManager;

Game::Game() :
		_mngr(), //
		_pacmanSys(), //
		_gameCtrlSys(), //
		_startsSys(), //
		_renderSys(), //
		_collisionSys() {

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

void Game::init() {

	// initialize the SDL singleton
	if (!SDLUtils::Init("PacMan, Stars, ...", 800, 600,
			"resources/config/resources.json")) {

		std::cerr << "Something went wrong while initializing SDLUtils"
				<< std::endl;
		return;
	}

	// initialize the InputHandler singleton
	if (!InputHandler::Init()) {
		std::cerr << "Something went wrong while initializing SDLHandler"
				<< std::endl;
		return;

	}

	// Create the manager
	_mngr = new EntityManager();

	// add the systems
	_pacmanSys = _mngr->addSystem<PacManSystem>();
	_startsSys = _mngr->addSystem<StarsSystem>();
	_gameCtrlSys = _mngr->addSystem<GameCtrlSystem>();
	_renderSys = _mngr->addSystem<RenderSystem>();
	_collisionSys = _mngr->addSystem<CollisionsSystem>();
}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;

	auto &ihdlr = ih();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		_pacmanSys->update();
		_startsSys->update();
		_gameCtrlSys->update();
		_collisionSys->update();

		_mngr->refresh();

		sdlutils().clearRenderer();
		_renderSys->update();
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}

}

