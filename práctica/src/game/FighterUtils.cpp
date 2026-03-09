#include "FighterUtils.h"
#include "Game.h"

#include "../components/Transform.h"
#include "../components/Image.h"
#include "../components/DeAcceleration.h"
#include "../components/Health.h"
#include "../components/FighterCtrl.h"
#include "../components/Gun.h"
#include "../components/ShowAtOpposieSide.h"

FighterUtils::FighterUtils() : fighter_(nullptr) {

}

FighterUtils::~FighterUtils() {}

void FighterUtils::create_fighter() {

	auto mngr = Game::Instance()->getMngr();

	fighter_ = mngr->addEntity(ecs::hdlr::FIGHTER);
	mngr->setHandler(ecs::hdlr::FIGHTER, fighter_);

	int w = sdlutils().width();
	int h = sdlutils().height();

	float fw = 50.0f;
	float fh = 50.0f;

	// Colocar el centro del fighter en el centro de la ventana
	Vector2D pos(w / 2.0f - fw / 2.0f, h / 2.0f - fh / 2.0f);

	fighter_->addComponent<Transform>(
		pos,
		Vector2D(0.0f, 0.0f),
		fw,
		fh,
		0.0f
	);

	fighter_->addComponent<Image>(
		&sdlutils().images().at("fighter")
	);

	fighter_->addComponent<DeAcceleration>();
	fighter_->addComponent<Health>();
	fighter_->addComponent<FighterCtrl>();
	fighter_->addComponent<Gun>();
	fighter_->addComponent<ShowAtOpposieSide>();
}

void FighterUtils::reset_fighter() {

	if (fighter_ == nullptr) return;

	auto tr = fighter_->getComponent<Transform>();
	auto gun = fighter_->getComponent<Gun>();

	int w = sdlutils().width();
	int h = sdlutils().height();

	float fw = tr->getWidth();
	float fh = tr->getHeight();

	tr->setPos(Vector2D(w / 2.0f - fw / 2.0f, h / 2.0f - fh / 2.0f));

	tr->setRot(0.0f);

	gun->reset();
}

void FighterUtils::reset_lives() {

	if (fighter_ == nullptr) return;

	auto health = fighter_->getComponent<Health>();
	health->resetLives();
}

int FighterUtils::update_lives(int n) {

	auto health = fighter_->getComponent<Health>();
	health->subLives(n);
	if (health->getLives() > 0)
		Game::Instance()->setState(Game::NEWROUND);
	else Game::Instance()->setState(Game::GAMEOVER_LOST);
	return health->getLives();
}