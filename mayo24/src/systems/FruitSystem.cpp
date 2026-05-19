// This file is part of the course TPV2@UCM - Samir Genaim

#include "FruitSystem.h"

#include <algorithm>
#include "../components/Image.h"
#include "../components/Points.h"
#include "../components/StarMotion.h"
#include "../components/Miracle.h"
#include "../components/Transform.h"
#include "../components/ImageWithFrames.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "GameCtrlSystem.h"
#include "../game/Game.h"

FruitSystem::FruitSystem() :
		_starsLimit(48), _currNumOfStars(0), _starPosX(30), _starPosY(30) {
}

FruitSystem::~FruitSystem() {
}

void FruitSystem::initSystem() {

}

void FruitSystem::update() {
	auto currTime = sdlutils().virtualTimer().currTime()/1000.0f;
	auto stars = _mngr->getEntities(ecs::grp::FRUITS);

	for (auto e : stars) {
		auto miracle = _mngr->getComponent<Miracle>(e);

		// Si no tiene componente Miracle, es una estrella normal y nunca cambia
		if (miracle == nullptr) {
			continue;
		}

		auto anim = _mngr->getComponent<ImageWithFrames>(e);

		if (miracle->switchState(currTime)) {
			if (miracle->isMiraculous()) {
				anim->setFrames({ 12 });  // Frame para estado milagroso
			}
			else {
				anim->setFrames({ 15 });  // Frame para estado normal
			}
		}
	}

}

void FruitSystem::addFruit(unsigned int n) {

	// Always use the random number generator provided by SDLUtils
	//
	auto &rand = sdlutils().rand();

	for (auto i = 0u; i < 6; i++) {
		for (auto j = 0u;j < 8;j++) {

			int m = rand.nextInt(1, 10);
			// add and entity to the manager
			//
			auto e = _mngr->addEntity(ecs::grp::FRUITS);

			// add a Transform component, and initialise it with random
			// size and position
			//
			auto tr = _mngr->addComponent<Transform>(e);
			tr->init(Vector2D(_starPosX, _starPosY), Vector2D(), 30, 30, 0.0f);

			// add an Image Component
			//
			auto tex = &sdlutils().images().at("sprites");
			auto anim =_mngr->addComponent<ImageWithFrames>(e,tex,8,8,100);
			anim->setFrames({ 12 });

			// add a StarMotion component to resize/rotate the star
			//
			auto motion = _mngr->addComponent<StarMotion>(e);
			if (m == 1) _mngr->addComponent<Miracle>(e);

			auto pts = _mngr->addComponent<Points>(e);
			pts->_points = rand.nextInt(1, 5);

			_starPosX += 100;
			
			_currNumOfStars++;
		}
		_starPosY += 100;
		_starPosX = 30;
	}
}

void FruitSystem::onFruitEaten(ecs::entity_t e) {

	auto miracle = _mngr->getComponent<Miracle>(e);
	bool isMiraculous = (miracle != nullptr);


	if (isMiraculous) {
		Message immunityMsg;
		immunityMsg.id = _m_IMMUNITY_START;
		_mngr->send(immunityMsg);
	}

	_mngr->setAlive(e, false);
	_currNumOfStars--;

	if(_currNumOfStars <= 0) {
		Game::Instance()->setState(Game::GAMEOVER_WON);
		sdlutils().soundEffects().at("pacman_won").play("se");
	}

	// play sound on channel 1 (if there is something playing there
	// it will be cancelled
	sdlutils().soundEffects().at("pacman_eat").play("se");
}

void FruitSystem::resetTimers() {
	auto stars = _mngr->getEntities(ecs::grp::FRUITS);
	Uint32 now = sdlutils().virtualTimer().currTime();

	for (auto e : stars) {
		auto miracle = _mngr->getComponent<Miracle>(e);
		if (miracle != nullptr) {  // Solo resetear las milagrosas
			miracle->reset(now);
		}
	}
}

void FruitSystem::clearAllFruit() {
	auto stars = _mngr->getEntities(ecs::grp::FRUITS);

	// Marcar todas las estrellas como muertas
	for (auto e : stars) {
		_mngr->setAlive(e, false);
	}

	// Limpiar el vector de estrellas (opcional, depende de tu ECS)
	stars.clear();

	// Reiniciar el contador
	_currNumOfStars = 0;

	// Reiniciar posiciones para que las nuevas estrellas se creen desde el principio
	_starPosX = 10;
	_starPosY = 10;

}

void FruitSystem::recieve(const Message &m) {
	switch (m.id) {
	case _m_PACMAN_FOOD_COLLISION:
		onFruitEaten(m.fruit_eaten_data.e);
		break;
	case _m_CREATE_FRUIT:
		addFruit(m.create_fruits_data.n);
		break;
	case _m_ROUND_START:
		resetTimers();
		break;
	case _m_NEW_GAME:
		clearAllFruit();
		_mngr->refresh();
		addFruit(m.create_fruits_data.n);
		resetTimers();
		break;
	default:
		break;
	}
}
