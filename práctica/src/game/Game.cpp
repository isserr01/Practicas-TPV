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
#include "../components/Gun.h"


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
	auto ship = _mngr->getHandler(ecs::hdlr::FIGHTER);
    auto asteroids = _mngr->getEntities(ecs::grp::ASTEROIDS);
	auto gun = ship->getComponent<Gun>();

    Transform* shipTr = ship->getComponent<Transform>();
    assert(shipTr != nullptr);

    bool shipHit = false;

    for (auto it : asteroids) {
        Transform* tr = it->getComponent<Transform>();
        assert(tr != nullptr);

        if (Collisions::collidesWithRotation(shipTr->getPos(), shipTr->getWidth(), shipTr->getHeight(), shipTr->getRot(), tr->getPos(), tr->getWidth(), tr->getHeight(), tr->getRot())) {
            // matarse ;(
            shipHit = true;
			std::cout << "colision" << std::endl;
           /* auto sound = &SDLUtils::instance()->soundEffects().at(sounds[EXPLOSION1]);
            sound->setVolume(shipDestroyVolume);
            sound->play();*/
            break;
        }

        for (auto &b : *gun) {
			if (!b.used)continue;

			if (Collisions::collides(tr->getPos(), tr->getWidth(), tr->getHeight(),
				b.pos, b.width, b.height)) 
			{
				// destruir bala
				b.used = false;
				_asteroidsFacade->split_astroid(it);

				break;
			}

          
                // dividir asteroide
                //astMngr->onCollision(it);
               /* auto sound = &SDLUtils::instance()->soundEffects().at(sounds[EXPLOSION]);
                sound->setVolume(asteroidDestroyVolume);
                sound->play();*/
               
            
        }
    }

   /* if (shipHit) {
        reset();
    }*/


}
