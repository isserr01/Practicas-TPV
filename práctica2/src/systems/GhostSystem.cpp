// This file is part of the course TPV2@UCM - Samir Genaim

#include "GhostSystem.h"

#include "../components/Image.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "PacManSystem.h"

#include <cstdlib>

GhostSystem::GhostSystem() :
	gmTR(nullptr),timerGhost(5.0f),immunity(false), cantidadGhosts(0) {
}

GhostSystem::~GhostSystem() {
}

void GhostSystem::spawnGhost() {
	// create the Ghost entity
	auto ghost = _mngr->addEntity(ecs::grp::GHOSTS);

	auto s = 50.0f;
	auto x = sdlutils().width();
	auto y = sdlutils().height();
	Vector2D pos;
	int r = rand() % 4;
	switch (r)
	{
		case 0: pos = Vector2D(0, 0); break;
		case 1: pos = Vector2D(x - s, 0); break;
		case 2: pos = Vector2D(0, y - s); break;
		case 3: pos = Vector2D(x - s, y - s); break;
	}

	auto tr = _mngr->addComponent<Transform>(ghost);
	tr->init(pos, Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(ghost, &sdlutils().images().at("pacman"));
	ghosts.push_back(ghost);
}

void GhostSystem::update() {
	if (!immunity && timerGhost >= 100.0f && cantidadGhosts < 10) {
		std::cout << "ENTRA IF" << std::endl;
		spawnGhost();
		timerGhost = 0.0f;
		cantidadGhosts++;
	}
	auto pacman = _mngr->getHandler(ecs::hdlr::PACMAN);
	auto pmTR = _mngr->getComponent<Transform>(pacman);
	Vector2D pmPos = pmTR->getPos();
	for (auto g : ghosts) {

		auto tr = _mngr->getComponent<Transform>(g);

		float r =(float) rand() / RAND_MAX;
		if (r < 0.005f) {
			auto pacPos= (pmPos - tr->getPos()).normalize();
			tr->_vel= pacPos * 1.1f;
		}
		tr->_pos = tr->_pos + tr->_vel;


		//q rebote en las paredes
		float x = tr->getPos().getX();
		float y = tr->getPos().getY();
		float w = sdlutils().width();
		float h = sdlutils().height();

		if (x <= 0 || x >= w)
			tr->_vel.setX(-(tr->_vel.getX()));

		if (y <= 0 || y >= h)
			tr->_vel.setY(-(tr->_vel.getY()));
	}
	std::cout << "ANTES" << timerGhost << std::endl;
	timerGhost ++; 
	
}
