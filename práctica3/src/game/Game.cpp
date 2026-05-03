// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "Bullets.h"
#include "LittleWolf.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"
#include "Networking.h"

Game::Game() :
	_bm(nullptr), //
	_wolf(nullptr), //
	_net(nullptr) {
}

Game::~Game() {
	delete _wolf;
	delete _bm;
	delete _net;

	// close SDLNet
	SDLNetUtils::close_SDLNet();

	// release InputHandler if the instance was created correctly.
	if (InputHandler::HasInstance())
		InputHandler::Release();

	// release SLDUtil if the instance was created correctly.
	if (SDLUtils::HasInstance())
		SDLUtils::Release();
}

bool Game::init() {
	// initialize the SDL singleton
	if (!SDLUtils::Init("LittleWolf Network", 800, 600,
		"resources/config/littlewolf.resources.json")) {
		std::cerr << "Something went wrong while initializing SDLUtils" << std::endl;
		return false;
	}

	// initialize the InputHandler singleton
	if (!InputHandler::Init()) {
		std::cerr << "Something went wrong while initializing SDLHandler" << std::endl;
		return false;
	}

	// initialize SDLNet
	SDLNetUtils::init_SDLNet();

	return true;
}

bool Game::init_game(const char* host, Uint16 port, const char* playerName) {
	_net = new Networking();

	// establish connection to server first
	if (!_net->init(host, port))
		return false;

	_bm = new Bullets();
	_wolf = new LittleWolf();

	// load a map
	_wolf->load("resources/maps/little_wolf/map_0.json");

	// initialize SDL window and renderer
	_wolf->init(sdlutils().window(), sdlutils().renderer());

	// add the local player
	_wolf->addPlayer(_net->get_client_id());
	_wolf->setMyPlayerId(_net->get_client_id());

	std::cout << "Connected with id " << (int)_net->get_client_id()
		<< " (master is " << (int)_net->get_master_id() << ")" << std::endl;

	return true;
}

void Game::start() {
	// a boolean to exit the loop
	bool exit = false;

	auto& ihdlr = ih();

	auto& vt = sdlutils().virtualTimer();
	vt.resetTime();

	while (!exit) {
		Uint32 startTime = vt.regCurrTime();

		// refresh the input handler
		ihdlr.refresh();
		if (ihdlr.keyDownEvent()) {

			// ESC exists the game
			if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
				exit = true;
				continue;
			}

			// R restarts the game (only master)
			if (ihdlr.isKeyDown(SDL_SCANCODE_R) && _net->is_master()) {
				_net->send_restart();
			}
		}

		// update networking (receive messages)
		_net->update();

		// update LittleWolf (detects inputs)
		_wolf->update();

		// send player state to server
		sendPlayerState();

		// check and send shoot
		checkAndSendShoot();

		// update bullets
		_bm->update();

		// check collisions (only master)
		check_collisions();

		// render
		sdlutils().clearRenderer();
		_wolf->render();
		_bm->render();
		sdlutils().presentRenderer();

		Uint32 frameTime = vt.currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}

	_net->disconnect();
}

void Game::sendPlayerState() {
	if (_wolf->getLocalState() != LittleWolf::ALIVE) return;

	LittleWolf::Point pos = _wolf->getLocalPosition();
	float theta = _wolf->getLocalTheta();

	// Enviar estado a través de Networking
	_net->send_state(Vector2D(pos.x, pos.y), 0, 0, theta);
}

void Game::checkAndSendShoot() {
	if (_wolf->isShootRequested()) {
		LittleWolf::Point origin = _wolf->getLocalPosition();
		float theta = _wolf->getLocalTheta();

		_net->send_shoot(Vector2D(origin.x, origin.y),
			Vector2D(cosf(theta), sinf(theta)),
			5, 5, theta);
		_wolf->clearShootRequest();
	}
}

void Game::check_collisions() {
	// Only the master checks collisions
	if (!_net->is_master()) return;

	for (Bullets::Bullet& b : *_bm) {
		if (b.used) {
			LittleWolf::Point origin(b.pos.getX(), b.pos.getY());
			float dirX = b.vel.getX();
			float dirY = b.vel.getY();

			Uint8 hitId;
			if (_wolf->checkCollision(origin, dirX, dirY, b.shooter, hitId)) {
				b.used = false;
				_net->send_dead(hitId, b.shooter, b.timestamp);
			}
		}
	}
}