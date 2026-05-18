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
#include "../systems/FruitSystem.h"
#include "../systems/ImmunitySystem.h"
#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"

void RunningState::update() {
	auto* game = Game::Instance();
	auto* gm = game->getMngr();
	auto& ihdlr = ih();
	auto& vt = sdlutils().virtualTimer();

	sdlutils().clearRenderer();

	if (ihdlr.keyDownEvent() && ihdlr.isKeyDown(SDL_SCANCODE_P)) {
		Game::Instance()->setState(Game::PAUSED);
	}

	for (auto sys : systems) {
		sys->update();
	}

	sdlutils().presentRenderer();
}

void RunningState::enter()
{
	if (systems.empty()) {
		_pacmanSys = Game::Instance()->getMngr()->addSystem<PacManSystem>();
		_starsSys = Game::Instance()->getMngr()->addSystem<FruitSystem>();
		_ghostSys = Game::Instance()->getMngr()->addSystem<GhostSystem>();
		_gameCtrlSys = Game::Instance()->getMngr()->addSystem<GameCtrlSystem>();
		_renderSys = Game::Instance()->getMngr()->addSystem<RenderSystem>();
		_collisionSys = Game::Instance()->getMngr()->addSystem<CollisionsSystem>();
		_immunitySys = Game::Instance()->getMngr()->addSystem<ImmunitySystem>();

		systems.push_back(_pacmanSys);
		systems.push_back(_starsSys);
		systems.push_back(_ghostSys);
		systems.push_back(_renderSys);
		systems.push_back(_gameCtrlSys);
		systems.push_back(_collisionSys);
		systems.push_back(_immunitySys);
	}


	Message m;
	m.id = _m_CREATE_FRUIT;
	m.create_fruits_data.n = 6;
	Game::Instance()->getMngr()->send(m);

}


