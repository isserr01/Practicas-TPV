// This file is part of the course TPV2@UCM - Samir Genaim

#include "PacManSystem.h"

#include "../components/Transform.h"
#include "../components/Health.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/ImageWithFrames.h"

PacManSystem::PacManSystem() :
		_pmTR(nullptr) {
}

PacManSystem::~PacManSystem() {
}

void PacManSystem::initSystem() {
	// create the PacMan entity
	//
	auto pacman = _mngr->addEntity();
	_mngr->setHandler(ecs::hdlr::PACMAN, pacman);

	_pmTR = _mngr->addComponent<Transform>(pacman);
	_mngr->addComponent<Health>(pacman);
	auto s = 50.0f;
	auto x = (sdlutils().width() - s) / 2.0f;
	auto y = (sdlutils().height() - s) / 2.0f;
	_pmTR->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);
	auto tex = &sdlutils().images().at("sprites");

	auto anim = _mngr->addComponent<ImageWithFrames>(pacman, tex, 8, 8, 0.1f);
	anim->setFrames({ 0, 1, 2, 3 });
}

void PacManSystem::update() {

	auto& ihldr = ih();

	if (ihldr.keyDownEvent()) {

		if (ihldr.isKeyDown(SDL_SCANCODE_RIGHT)) { // rotate right
			_pmTR->_rot = _pmTR->_rot + 90.0f;

			// also rotate the PacMan so it looks in the same
			// direction where it moves
			//
			_pmTR->_vel = _pmTR->_vel.rotate(90.0f);
		}
		else if (ihldr.isKeyDown(SDL_SCANCODE_LEFT)) { // rotate left
			_pmTR->_rot = _pmTR->_rot - 90.0f;

			// also rotate the PacMan so it looks in the same
			// direction where it moves
			//
			_pmTR->_vel = _pmTR->_vel.rotate(-90.0f);
		}
		else if (ihldr.isKeyDown(SDL_SCANCODE_UP)) { // increase speed

			// add 1.0f to the speed (respecting the limit 3.0f). Recall
			// that speed is the length of the velocity vector
			float speed = std::min(3.0f, _pmTR->_vel.magnitude() + 1.0f);

			// change the length of velocity vecto to 'speed'. We need
			// '.rotate(rot)' for the case in which the current speed is
			// 0, so we rotate it to the same direction where the PacMan
			// is looking
			//
			_pmTR->_vel = Vector2D(0, -speed).rotate(_pmTR->_rot);
		}
		else if (ihldr.isKeyDown(SDL_SCANCODE_DOWN)) { // decrease speed
			// subtract 1.0f to the speed (respecting the limit 0.0f). Recall
			// that speed is the length of the velocity vector
			float speed = std::max(0.0f, _pmTR->_vel.magnitude() - 1.0f);

			// change the length of velocity vector to 'speed'. We need
			// '.rotate(rot)' for the case in which the current speed is
			// 0, so we rotate it to the same direction where the PacMan
			// is looking
			//
			_pmTR->_vel = Vector2D(0, -speed).rotate(_pmTR->_rot);
		}



	}

	// move the pacman
	_pmTR->_pos = _pmTR->_pos + _pmTR->_vel;

	// check left/right borders
	if (_pmTR->_pos.getX() < 0) {
		_pmTR->_pos.setX(0.0f);
		_pmTR->_vel.set(0.0f, 0.0f);
	}
	else if (_pmTR->_pos.getX() + _pmTR->_width > sdlutils().width()) {
		_pmTR->_pos.setX(sdlutils().width() - _pmTR->_width);
		_pmTR->_vel.set(0.0f, 0.0f);
	}

	// check upper/lower borders
	if (_pmTR->_pos.getY() < 0) {
		_pmTR->_pos.setY(0.0f);
		_pmTR->_vel.set(0.0f, 0.0f);
	}
	else if (_pmTR->_pos.getY() + _pmTR->_height > sdlutils().height()) {
		_pmTR->_pos.setY(sdlutils().height() - _pmTR->_height);
		_pmTR->_vel.set(0.0f, 0.0f);
	}

	if (!_shieldTR.empty()) {
		for (auto i = 0u; i < _shieldTR.size(); i++) {
			// Calcular offset basado en el índice
			Vector2D posDiff;

			switch (i) {
			case 0:  // estrella derecha
				posDiff = Vector2D(40, 0);
				break;
			case 1:  // estrella izquierda
				posDiff = Vector2D(-40, 0);
				break;
			case 2:  // estrella abajo
				posDiff = Vector2D(0, 40);
				break;
			case 3:  // estrella arriba
				posDiff = Vector2D(0, -40);
				break;
			default:
				posDiff = Vector2D(0, 0);
				break;
			}

			// Calcular nueva posición
			Vector2D newPos = _pmTR->_pos + posDiff;

			// Actualizar el Transform de la estrella
			if (_shieldTR[i] != nullptr) {
				_shieldTR[i]->_pos = newPos;
			}
		}
	}


}

void PacManSystem::recieve(const Message& m) {
	Health* health = _mngr->getComponent<Health>(_mngr->getHandler(ecs::hdlr::PACMAN));
	switch (m.id) {
	case _m_NEW_GAME:
		health->resetLives();
		_pmTR->_pos = Vector2D((sdlutils().width() - _pmTR->_width) / 2.0f,
			(sdlutils().height() - _pmTR->_height) / 2.0f);
		_pmTR->_vel = Vector2D();
		_pmTR->_rot = 0.0f;
		break;
	case _m_PACMAN_GHOST_COLLISION:		
		Game::Instance()->setState(Game::NEWROUND);
		break;
	case _m_SHIELD_ACTIVATED:
			activateShield();
			break;
	case _m_SHIELD_DEACTIVATED:
		deactivateShield();
		break;
	case _m_ROUND_OVER:
		deactivateShield();
	}

	
}

void PacManSystem::activateShield() {
	
	for (auto i = 0u; i < 4; i++) {

		Vector2D posDiff = Vector2D(0, 0);
		if (i % 2 == 0) {
			posDiff.setX(40);
			posDiff.setY(0);
		}
		else {
			posDiff.setX(0);
			posDiff.setY(40);
		}

		// add and entity to the manager
		//
		auto e = _mngr->addEntity(ecs::grp::STARS);

		// add a Transform component, and initialise it with random
		// size and position
		//
		auto tr = _mngr->addComponent<Transform>(e);

		if (i == 0 || i == 2) posDiff;
		else if (i == 1 || i == 3) posDiff = posDiff * -1;

		Vector2D pos = _pmTR->getPos() + posDiff;

		tr->init(pos, Vector2D(), 40, 40, 0.0f);

		// add an Image Component
		//
		auto tex = &sdlutils().images().at("sprites");
		auto anim = _mngr->addComponent<ImageWithFrames>(e, tex, 8, 8, 100);
		anim->setFrames({ 7 });

		_shieldTR.push_back(tr);

	}

	std::cout << "Shield activated!" << std::endl;


}

void PacManSystem::deactivateShield() {

	auto stars = _mngr->getEntities(ecs::grp::STARS);

	// Marcar todas las estrellas como muertas
	for (auto e : stars) {
		_mngr->setAlive(e, false);
	}

	// Limpiar el vector de estrellas (opcional, depende de tu ECS)
	stars.clear();

}


