#include "RunningState.h"
#include "../game/Game.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/GameCtrlSystem.h"
#include "../systems/PacManSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/GhostSystem.h"
#include "../systems/StarsSystem.h"
#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"

void RunningState::update() {
	auto* game = Game::Instance();
	auto* gm = game->getMngr();
	auto& ihdlr = ih();
	auto& vt = sdlutils().virtualTimer();

	sdlutils().clearRenderer();

	//gm->update();
	//game->checkCollisions();
	//gm->render();
	//gm->refresh();
	//if (vt.currTime() - _lastAsteroidTime >= 5000) {
	//	Game::Instance()->getAteroidsFacade()->create_asteroids(1);
	//	_lastAsteroidTime = vt.currTime();
	//}

	if (ihdlr.keyDownEvent() && ihdlr.isKeyDown(SDL_SCANCODE_P)) {
		Game::Instance()->setState(Game::PAUSED);
	}

	//Game::Instance()->getMngr()->render();

	for (auto sys : systems) {
		sys->update();
	}

	sdlutils().presentRenderer();
}

void RunningState::enter()
{
	// add the systems
	_pacmanSys = Game::Instance()->getMngr()->addSystem<PacManSystem>();
	_starsSys = Game::Instance()->getMngr()->addSystem<StarsSystem>();
	_ghostSys = Game::Instance()->getMngr()->addSystem<GhostSystem>();
	_gameCtrlSys = Game::Instance()->getMngr()->addSystem<GameCtrlSystem>();
	_renderSys = Game::Instance()->getMngr()->addSystem<RenderSystem>();
	_collisionSys = Game::Instance()->getMngr()->addSystem<CollisionsSystem>();

	systems.push_back(_pacmanSys);
	systems.push_back(_starsSys);
	systems.push_back(_ghostSys);
	systems.push_back(_renderSys);
	systems.push_back(_gameCtrlSys);
	systems.push_back(_collisionSys);

	Message m;
	m.id = _m_CREATE_STARS;
	m.create_stars_data.n = 5;
	Game::Instance()->getMngr()->send(m);
		
	

}

void RunningState::receive(const Message& m)
{
	switch (m.id) {
	case _m_ROUND_OVER:
		Game::Instance()->setState(Game::PAUSED);
		break;
	default:
		break;
	}
}

