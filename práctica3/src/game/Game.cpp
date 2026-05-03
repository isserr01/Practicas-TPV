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
	_net(nullptr),
	_clientId(0),
	_masterId(0),
	_running(true),
	_lastStateSend(0),
	_lastSentX(3.5f),
	_lastSentY(3.5f),
	_lastSentTheta(0) {
}

Game::~Game() {
	delete _wolf;
	delete _bm;
	delete _net;

	SDLNetUtils::close_SDLNet();

	if (InputHandler::HasInstance())
		InputHandler::Release();
	if (SDLUtils::HasInstance())
		SDLUtils::Release();
}

bool Game::init() {
	if (!SDLUtils::HasInstance()) {
		if (!SDLUtils::Init("LittleWolf Network", 800, 600,
			"resources/config/littlewolf.resources.json")) {
			std::cerr << "Error initializing SDLUtils" << std::endl;
			return false;
		}
	}

	if (!InputHandler::Init()) {
		std::cerr << "Error initializing InputHandler" << std::endl;
		return false;
	}

	SDLNetUtils::init_SDLNet();
	return true;
}

bool Game::init_game(const char* host, Uint16 port, const char* playerName) {
	_playerName = playerName;

	if (!init()) return false;

	// 1. Conectar al servidor
	_net = new Networking();
	if (!_net->init(host, port)) {
		std::cerr << "Failed to connect to server" << std::endl;
		return false;
	}

	_clientId = _net->get_client_id();
	_masterId = _net->get_master_id();

	// 2. Inicializar LittleWolf
	_wolf = new LittleWolf();
	_wolf->load("resources/maps/little_wolf/map_0.json");
	_wolf->init(sdlutils().window(), sdlutils().renderer());

	// 3. Configurar el jugador local
	_wolf->setMyPlayerId(_clientId);
	_wolf->setMaster(_clientId == _masterId);

	// 4. Añadir el jugador local al juego
	_wolf->addPlayer(_clientId);

	// 5. Inicializar balas (opcional, para efectos visuales)
	_bm = new Bullets();

	std::cout << "Connected with id " << (int)_clientId
		<< " (master is " << (int)_masterId << ")" << std::endl;

	return true;
}

void Game::start() {
	auto& ihdlr = ih();
	auto& vt = sdlutils().virtualTimer();
	vt.resetTime();

	while (_running) {
		Uint32 startTime = vt.regCurrTime();

		ihdlr.refresh();

		if (ihdlr.keyDownEvent()) {
			if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
				_running = false;
				continue;
			}
			// R para reiniciar (solo master)
			if (ihdlr.isKeyDown(SDL_SCANCODE_R) && _net->is_master()) {
				_net->send_restart();
			}
		}

		// 1. Recibir mensajes de red
		_net->update();

		// 2. Detectar inputs locales (sin mover todavía)
		_wolf->update();  // Este método ya detecta inputs y los guarda en _pendingMoveX/Y/RotDelta

		// 3. Enviar estado al servidor
		sendPlayerState();
		checkAndSendShoot();

		// 4. Actualizar balas
		_bm->update();

		// 5. Verificar colisiones (solo el master)
		check_collisions();

		// 6. Renderizar
		sdlutils().clearRenderer();
		_wolf->render();
		_bm->render();
		sdlutils().presentRenderer();

		Uint32 frameTime = vt.currRealTime() - startTime;
		if (frameTime < 16) SDL_Delay(16 - frameTime);
	}

	_net->disconnect();
}

void Game::sendPlayerState() {
	// Solo enviar si estamos vivos
	if (_wolf->getLocalState() != LittleWolf::ALIVE) return;

	Uint32 now = SDL_GetTicks();
	if (now - _lastStateSend < 50) return;

	// Obtener inputs pendientes (ya detectados por LittleWolf::update())
	float moveX = _wolf->getPendingMoveX();
	float moveY = _wolf->getPendingMoveY();
	float rotDelta = _wolf->getPendingRotDelta();

	float speed = 0.05f;
	float rotSpeed = 0.05f;

	// Calcular nueva posición (simulación local)
	float newTheta = _lastSentTheta + rotDelta * rotSpeed;
	float newX = _lastSentX + (moveY * cosf(_lastSentTheta) - moveX * sinf(_lastSentTheta)) * speed;
	float newY = _lastSentY + (moveY * sinf(_lastSentTheta) + moveX * cosf(_lastSentTheta)) * speed;

	// Solo enviar si hubo cambio significativo
	if (fabs(newX - _lastSentX) > 0.01f || fabs(newY - _lastSentY) > 0.01f ||
		fabs(newTheta - _lastSentTheta) > 0.01f) {

		// Enviar estado al servidor
		_net->send_state(Vector2D(newX, newY), 0, 0, newTheta);

		// Actualizar posición local visual
		_wolf->updatePlayerPosition(_clientId, newX, newY, newTheta);

		_lastSentX = newX;
		_lastSentY = newY;
		_lastSentTheta = newTheta;
		_lastStateSend = now;
	}
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
	// Solo el master verifica colisiones
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