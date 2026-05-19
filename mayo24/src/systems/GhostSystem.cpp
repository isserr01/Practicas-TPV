// This file is part of the course TPV2@UCM - Samir Genaim

#include "GhostSystem.h"

#include "../components/ImageWithFrames.h"
#include "../components/Health.h"
#include "../components/Weak.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../game/Game.h"
#include "../sdlutils/SDLUtils.h"
#include "PacManSystem.h"


#include <cstdlib>

GhostSystem::GhostSystem() :
	gmTR(nullptr),timerGhost(5.0f),immunity(false), cantidadGhosts(0) {
}

GhostSystem::~GhostSystem() {
}

void GhostSystem::spawnGhost() {

	auto& randm = sdlutils().rand();
	int m = randm.nextInt(1, 5);
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
	
	auto anim = _mngr->addComponent<ImageWithFrames>(ghost, &sdlutils().images().at("sprites"),8,8,0.1f);

	if (m == 1) {
		_mngr->addComponent<Weak>(ghost,tr->getPos());
		//fantasma verde
		anim->setFrames({ 40, 41, 42, 43, 44, 45, 46, 47 });
		std::cout << "Fantasma débil creado" << std::endl;
	}
	else {
		//fantasma rojo
		anim->setFrames({ 32, 33, 34, 35,36,37,38,39 });
	}

}

void GhostSystem::update() {

	if (!_safeMode) {

		if (!immunity && timerGhost >= 100.0f && cantidadGhosts < 10) {
			spawnGhost();
			timerGhost = 0.0f;
			cantidadGhosts++;
		}
		auto pacman = _mngr->getHandler(ecs::hdlr::PACMAN);
		auto pmTR = _mngr->getComponent<Transform>(pacman);


		Vector2D pmPos = pmTR->getPos();

		auto& ghosts = _mngr->getEntities(ecs::grp::GHOSTS);

		for (auto g : ghosts) {
			if (immunity) {
				auto anim = _mngr->getComponent<ImageWithFrames>(g);
				anim->setFrames({ 48, 49, 50, 51,52,53,54,55 });
			}
			else {
				auto anim = _mngr->getComponent<ImageWithFrames>(g);
				anim->setFrames({ 32, 33, 34, 35,36,37,38,39 });
			}
			auto tr = _mngr->getComponent<Transform>(g);

			float r = (float)rand() / RAND_MAX;
			if (r < 0.005f) {
				auto pacPos = (pmPos - tr->getPos()).normalize();
				tr->_vel = pacPos * 1.1f;
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
		timerGhost++;
	}
	
	
}
void GhostSystem::weakGhostCollision(ecs::entity_t e)
{
	auto tr = _mngr->getComponent<Transform>(e);
	Vector2D birthPos = _mngr->getComponent<Weak>(e)->getPos();

	tr->getPos().setX(birthPos.getX());
	tr->getPos().setY(birthPos.getY());

}
//colision entre pacman y fantasma (siendo inmortal)
void GhostSystem::colisionPac(ecs::entity_t e) {

	Health* health = _mngr->getComponent<Health>(_mngr->getHandler(ecs::hdlr::PACMAN));
	if (health != nullptr) {
		health->subLives(1);

		auto pacman = _mngr->getHandler(ecs::hdlr::PACMAN);
		auto pmTR = _mngr->getComponent<Transform>(pacman);
		auto s = pmTR->_width;
		pmTR->_pos = Vector2D((sdlutils().width() - s) / 2.0f,
			(sdlutils().height() - s) / 2.0f);
		pmTR->_vel = Vector2D(0, 0);

		if (health->getLives() <= 0) {
			Message m;
			m.id = _m_ROUND_OVER;
			Game::Instance()->getMngr()->send(m);
			sdlutils().soundEffects().at("pacman_death").play("se");
			Game::Instance()->setState(Game::GAMEOVER_LOST);
		}
		
	}
}

void GhostSystem::recieve(const Message& m) {
	switch (m.id) {
	case _m_PACMAN_GHOST_COLLISION:
		colisionPac(m.ghost_coll_data.e);
		break;
	case _m_IMMUNITY_START:
		immunity = true;
		break;
	case _m_IMMUNITY_END:
		immunity = false;
		break;
	case _m_ROUND_START:
		{
			immunity = false;
			auto& ghosts = _mngr->getEntities(ecs::grp::GHOSTS);
			for (auto g : ghosts) {
				_mngr->setAlive(g, false);
			}
			cantidadGhosts = 0;
			break;
		}
	case _m_GHOST_EATEN: 
		{
			// Matar fantasma
			_mngr->setAlive(m.ghost_coll_data.e, false);
			sdlutils().soundEffects().at("pacman_chomp").play("se");
			cantidadGhosts--;
			break;

		}
	case _m_WEAK_GHOST_COLLISION:
		{
			auto weak = _mngr->getComponent<Weak>(m.ghost_coll_data.e);
			if (weak != nullptr) {
				weak->subCrashes();
			}
			weakGhostCollision(m.ghost_coll_data.e);
			break;
		}
	case _m_SAFE_ACTIVATED:
		{
			_safeMode = true;
			break;
		}
	case _m_SAFE_DEACTIVATED:
		{
			_safeMode = false;
			break;
		}
	default:
		break;
	}
}

