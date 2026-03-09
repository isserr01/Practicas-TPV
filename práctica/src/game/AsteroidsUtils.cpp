#include "AsteroidsUtils.h"
#pragma once
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "Game.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/ImageWithFrames.h"
#include "../components/Generations.h"
#include "../components/ShowAtOpposieSide.h"
#include "../components/TeleportOnExit.h"
#include "../components/TowardDestination.h"
#include "../components/Follow.h"
#include "../components/MaterialConsistency.h"



AsteroidsUtils::AsteroidsUtils() {}

AsteroidsUtils::~AsteroidsUtils() {
}
void AsteroidsUtils::create_asteroids(int n) {
	for (int i = 0; i < n; i++) {
		auto mngr = Game::Instance()->getMngr();
		auto& rnd = sdlutils().rand();

		int w = sdlutils().width();
		int h = sdlutils().height();

		//posicion aleatoria
		Vector2D p;
		int side = rnd.nextInt(0, 3); //random arriba, abajo, izq y der
		switch (side) {
		case 0: p = Vector2D(rnd.nextInt(0, w), 0); break;
		case 1: p = Vector2D(rnd.nextInt(0, w), h); break;
		case 2: p = Vector2D(0, rnd.nextInt(0, h)); break;
		case 3: p = Vector2D(w, rnd.nextInt(0, h)); break;
		}
		//punto central
		Vector2D centro(w / 2.0f, h / 2.0f);
		Vector2D r(rnd.nextInt(-100, 100), rnd.nextInt(-100, 100));
		Vector2D c = centro + r;

		//velocidad
		float speed = rnd.nextInt(1, 5) / 40.0f;
		Vector2D v = (c - p).normalize() * speed;

		//tamaño
		int g = rnd.nextInt(1, 3);
		float size = 10.0f + 5.0f * g;

		auto a = mngr->addEntity(ecs::grp::ASTEROIDS);
		a->addComponent<Transform>(p, v, size, size, 0.0f);

		a->addComponent<ImageWithFrames>(
			&sdlutils().images().at("asteroid"), 6, 5, 50.0f
		);

		a->addComponent<Generations>(g);

		//random componentes
		if (rnd.nextInt(0, 1) == 0)
			a->addComponent<ShowAtOpposieSide>();
		else
			a->addComponent<TeleportOnExit>();

		int behavior = rnd.nextInt(0, 2);
		if (behavior == 0)
			a->addComponent<Follow>();
		else if (behavior == 1)
			a->addComponent<TowardDestination>();

		if (rnd.nextInt(0, 1) == 0) {
			int consistency = rnd.nextInt(10, 100);
			a->addComponent<MaterialConsistency>(consistency);
		}

	}

}

void AsteroidsUtils::remove_all_asteroids() {
	auto mngr = Game::Instance()->getMngr();
	auto& asteroids = mngr->getEntities(ecs::grp::ASTEROIDS);

	for (auto& a : asteroids) {
		if (a->isAlive()) a->setAlive(false);
	}
	mngr->refresh();
}

void AsteroidsUtils::split_astroid(ecs::Entity* a) {
	auto mngr = Game::Instance()->getMngr();
	auto& rnd = sdlutils().rand();
	//verificamos q esta vivo

	if (!a->isAlive()) return;

	auto tr = a->getComponent<Transform>();
	auto gens = a->getComponent<Generations>();
	if (!tr || !gens) return;

	int g = gens->getGenerations();

	if (g <= 0) {
		a->setAlive(false);
		return;
	}
	//guardamos todos los datos 
	Vector2D p = tr->getPos();
	Vector2D v = tr->getVel();
	float w = tr->getWidth();
	float h = tr->getHeight();

	MaterialConsistency* mc = nullptr;
	if (a->hasComponent<MaterialConsistency>())
		mc = a->getComponent<MaterialConsistency>();
	//lo destruimos 
	a->setAlive(false);

	//creamos dos nuevos
	for (int i = 0; i < 2; i++) {
		int r = rnd.nextInt(0, 360);

		Vector2D newPos = p + v.rotate(r) * 2 * std::max(w, h);
		Vector2D newVel = v.rotate(r) * 1.1f;

		//creacion nueva entidad
		auto nA = mngr->addEntity(ecs::grp::ASTEROIDS);
		int newG = g - 1;
		float size = 10.0f + 5.0f * newG;

		nA->addComponent<Transform>(p, v, size, size, 0.0f);

		nA->addComponent<ImageWithFrames>(
			&sdlutils().images().at("asteroid"), 6, 5, 50.0f
		);
		nA->addComponent<Generations>(newG);

		if (rnd.nextInt(0, 1) == 0)
			nA->addComponent<ShowAtOpposieSide>();
		else
			nA->addComponent<TeleportOnExit>();

		int behavior = rnd.nextInt(0, 2);
		if (behavior == 0)
			nA->addComponent<Follow>();
		else if (behavior == 1)
			nA->addComponent<TowardDestination>();

	}
}