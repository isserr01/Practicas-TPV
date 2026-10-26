#include "HelpersSystem.h"

#include "../components/ImageWithFrames.h"
#include "../components/Health.h"
#include "../components/Transform.h"
#include "../components/Mortal.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../game/Game.h"
#include "../sdlutils/SDLUtils.h"
#include "PacManSystem.h"
#include <cstdlib>

const int P = 10.0f;

HelpersSystem::HelpersSystem() :
	hlTR(nullptr), timerHelper(5.0f), cantidadHelpers(0) {
}


void HelpersSystem::spawnHelper() {

	auto& randm = sdlutils().rand();
	int m = randm.nextInt(1, P);

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
	//fantasma rojo
	auto anim = _mngr->addComponent<ImageWithFrames>(ghost, &sdlutils().images().at("sprites"), 8, 8, 0.1f);
	anim->setFrames({ 32, 33, 34, 35,36,37,38,39 });

	if (m == 1) {
		_mngr->addComponent<Mortal>(ghost, tr->getPos());
		std::cout << "Ghost mortal creado" << std::endl;
	}


}

void HelpersSystem::update() {

	if (cantidadHelpers <= N) {
		spawnHelper();
		cantidadHelpers++;
	}

	auto& ghosts = _mngr->getEntities(ecs::grp::GHOSTS);
	auto& helpers = _mngr->getEntities(ecs::grp::HELPERS);

	for (auto g : ghosts) {
		for (auto h : helpers) {

			Vector2D d = Vector2D(1000,1000);

			auto g_tr = _mngr->getComponent<Transform>(g);
			auto h_tr = _mngr->getComponent<Transform>(h);

			Vector2D distance = (g_tr->getPos() - h_tr->getPos());

			if(d.getX() > distance.getX() && d.getY() > distance.getY())
				d = distance;

			float r = (float)rand() / RAND_MAX;
			if (r < 0.005f) {
				auto ghostPos = (g_tr->getPos() - h_tr->getPos()).normalize()*V;
				h_tr->_vel = ghostPos * 1.1f;
			}
			h_tr->_pos = h_tr->_pos + h_tr->_vel;


			//q rebote en las paredes
			float x = h_tr->getPos().getX();
			float y = h_tr->getPos().getY();
			float w = sdlutils().width();
			float h = sdlutils().height();

			if (x <= 0 || x >= w)
				h_tr->_vel.setX(-(h_tr->_vel.getX()));

			if (y <= 0 || y >= h)
				h_tr->_vel.setY(-(h_tr->_vel.getY()));

		}

	}

}
//colision entre pacman y fantasma (siendo inmortal)
void HelpersSystem::colissionGhost(ecs::entity_t e) {

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

void HelpersSystem::recieve(const Message& m) {
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
		auto mortal = _mngr->getComponent<Mortal>(m.ghost_coll_data.e);

		if (mortal) {
			mortal->crash();
			auto tr = _mngr->getComponent<Transform>(m.ghost_coll_data.e);
			tr->_pos = mortal->getBirthPos();
			if (mortal->T <= 0) {
				_mngr->setAlive(m.ghost_coll_data.e, false);
				sdlutils().soundEffects().at("pacman_chomp").play("se");
				cantidadGhosts--;
			}
		}
		else {
			// Matar fantasma
			_mngr->setAlive(m.ghost_coll_data.e, false);
			sdlutils().soundEffects().at("pacman_chomp").play("se");
			cantidadGhosts--;
		}
		break;
	}
	default:
		break;
	}
}

