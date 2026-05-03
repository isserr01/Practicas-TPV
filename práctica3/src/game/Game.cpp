// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "Bullets.h"
#include "LittleWolf.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "SDLNetUtils.h"
#include "../utils/Collisions.h"

// Tipos de mensaje (deben coincidir con el servidor)
#define _CONN_REQUEST_ACCEPTED 1
#define _CLIENT_CONNECTED 2
#define _CLIENT_DISCONNECTED 3
#define _PLAYER_STATE 10
#define _PLAYER_INFO 11
#define _SHOOT 12
#define _DEAD 13
#define _RESTART 17

Game::Game() :
	_bm(nullptr), //
	_wolf(nullptr), //
	_socket(nullptr),
	_clientId(0),
	_masterId(0),
	_running(true),
	_lastStateSend(0),
	_lastSentX(3.5f),
	_lastSentY(3.5f),
	_lastSentTheta(0) {
}

Game::~Game() {
	if (_socket) {
		NET_DestroyStreamSocket(_socket);
	}
	delete _wolf;
	delete _bm;

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

	if(!InputHandler::HasInstance()) {
		if (!InputHandler::Init()) {
			std::cerr << "Error initializing InputHandler" << std::endl;
			return false;
		}
	}


	SDLNetUtils::init_SDLNet();
	return true;
}

bool Game::init_game(const char* host, Uint16 port, const char* playerName) {
	_playerName = playerName;

	if (!init()) return false;

	// Conectar al servidor
	if (!connectToServer(host, port)) {
		std::cerr << "Failed to connect to server" << std::endl;
		return false;
	}

	// Inicializar LittleWolf
	_wolf = new LittleWolf();
	_wolf->load("resources/maps/little_wolf/map_0.json");
	_wolf->init(sdlutils().window(), sdlutils().renderer());

	_wolf->setMyPlayerId(_clientId);
	_wolf->setMaster(_clientId == _masterId);
	_wolf->addPlayer(_clientId);

	_bm = new Bullets();

	std::cout << "Connected with id " << (int)_clientId
		<< " (master is " << (int)_masterId << ")" << std::endl;

	return true;
}

bool Game::connectToServer(const char* host, Uint16 port) {
	NET_Address* addr = NET_ResolveHostname(host);
	if (!addr) return false;

	if (NET_WaitUntilResolved(addr, 1000) != NET_SUCCESS) {
		NET_UnrefAddress(addr);
		return false;
	}

	_socket = NET_CreateClient(addr, port);
	NET_UnrefAddress(addr);

	if (!_socket) return false;

	if (NET_WaitUntilConnected(_socket, 5000) != NET_SUCCESS) {
		NET_DestroyStreamSocket(_socket);
		_socket = nullptr;
		return false;
	}

	// Esperar mensaje de aceptación
	void* conn[1] = { _socket };
	if (NET_WaitUntilInputAvailable(conn, 1, 5000) <= 0) {
		return false;
	}

	Uint8 buffer[256];
	int received = NET_ReadFromStreamSocket(_socket, buffer, sizeof(buffer));

	if (received >= 3 && buffer[0] == _CONN_REQUEST_ACCEPTED) {
		_clientId = buffer[1];
		_masterId = buffer[2];
		return true;
	}

	return false;
}

void Game::receiveMessages() {
	void* conn[1] = { _socket };
	if (NET_WaitUntilInputAvailable(conn, 1, 0) <= 0) return;

	Uint8 buffer[1024];
	int received = NET_ReadFromStreamSocket(_socket, buffer, sizeof(buffer));

	if (received <= 0) {
		if (received == -1) {
			std::cerr << "Connection error" << std::endl;
			_running = false;
		}
		return;
	}

	handleServerMessage(buffer, received);
}

void Game::handleServerMessage(const Uint8* data, size_t size) {
	if (size < 1) return;

	Uint8 type = data[0];

	switch (type) {
	case _CLIENT_CONNECTED:
		if (size >= 3) {
			handleNewClient(data[1], data[2]);
		}
		break;
	case _CLIENT_DISCONNECTED:
		if (size >= 3) {
			handleClientDisconnected(data[1], data[2]);
		}
		break;
	case _PLAYER_STATE:
		if (size >= 14) {
			handlePlayerState(data[1], data + 2);
		}
		break;
	case _PLAYER_INFO:
		if (size >= 18) {
			handlePlayerInfo(data[1], data + 2);
		}
		break;
	case _SHOOT:
		if (size >= 18) {
			handleShoot(data[1], data + 2);
		}
		break;
	case _DEAD:
		if (size >= 6) {
			handleDead(data[1], data + 2);
		}
		break;
	case _RESTART:
		handleRestart();
		break;
	default:
		std::cout << "Unknown message type: " << (int)type << std::endl;
		break;
	}
}

void Game::handleNewClient(Uint8 id, Uint8 masterId) {
	_masterId = masterId;
	// El nuevo cliente se añadirá cuando reciba PLAYER_INFO
}

void Game::handleClientDisconnected(Uint8 id, Uint8 masterId) {
	_masterId = masterId;
	// El servidor ya maneja esto
}

void Game::handlePlayerState(Uint8 clientId, const Uint8* data) {
	if (clientId == _clientId) return;

	float x, y, theta;
	memcpy(&x, data, 4);
	memcpy(&y, data + 4, 4);
	memcpy(&theta, data + 8, 4);

	_wolf->updatePlayerPosition(clientId, x, y, theta);
}

void Game::handlePlayerInfo(Uint8 clientId, const Uint8* data) {
	if (clientId == _clientId) return;

	float x, y, theta;
	Uint8 state, health;
	Sint32 score;
	char name[11];

	memcpy(&x, data, 4);
	memcpy(&y, data + 4, 4);
	memcpy(&theta, data + 8, 4);
	state = data[12];
	health = data[13];
	memcpy(&score, data + 14, 4);
	memcpy(name, data + 18, 10);
	name[10] = '\0';

	_wolf->updatePlayerRemote(clientId, x, y, theta, state, health, score, name);
}

void Game::handleShoot(Uint8 clientId, const Uint8* data) {
	// Por ahora solo registrar
	std::cout << "Shot from player " << (int)clientId << std::endl;
}

void Game::handleDead(Uint8 clientId, const Uint8* data) {
	Uint8 killerId = data[0];
	_wolf->onPlayerDied(clientId, killerId);
}

void Game::handleRestart() {
	_wolf->restartGame();
}

void Game::sendPlayerState() {
	if (_wolf->getLocalState() != LittleWolf::ALIVE) return;

	Uint32 now = SDL_GetTicks();
	if (now - _lastStateSend < 50) return;

	float moveX = _wolf->getPendingMoveX();
	float moveY = _wolf->getPendingMoveY();
	float rotDelta = _wolf->getPendingRotDelta();

	float speed = 0.05f;
	float rotSpeed = 0.05f;

	float newTheta = _lastSentTheta + rotDelta * rotSpeed;
	float newX = _lastSentX + (moveY * cosf(_lastSentTheta) - moveX * sinf(_lastSentTheta)) * speed;
	float newY = _lastSentY + (moveY * sinf(_lastSentTheta) + moveX * cosf(_lastSentTheta)) * speed;

	if (fabs(newX - _lastSentX) > 0.01f || fabs(newY - _lastSentY) > 0.01f ||
		fabs(newTheta - _lastSentTheta) > 0.01f) {

		// Construir mensaje PLAYER_STATE
		Uint8 buffer[1 + 1 + 4 + 4 + 4];
		buffer[0] = _PLAYER_STATE;
		buffer[1] = _clientId;
		memcpy(buffer + 2, &newX, 4);
		memcpy(buffer + 6, &newY, 4);
		memcpy(buffer + 10, &newTheta, 4);

		NET_WriteToStreamSocket(_socket, buffer, sizeof(buffer));

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

		Uint8 buffer[1 + 1 + 4 + 4 + 4 + 4];
		buffer[0] = _SHOOT;
		buffer[1] = _clientId;
		memcpy(buffer + 2, &origin.x, 4);
		memcpy(buffer + 6, &origin.y, 4);
		float dirX = cosf(theta);
		float dirY = sinf(theta);
		memcpy(buffer + 10, &dirX, 4);
		memcpy(buffer + 14, &dirY, 4);

		NET_WriteToStreamSocket(_socket, buffer, sizeof(buffer));

		_wolf->clearShootRequest();
	}
}

void Game::check_collisions() {
	// Solo el master verifica colisiones
	if (_clientId != _masterId) return;

	for (Bullets::Bullet& b : *_bm) {
		if (b.used) {
			LittleWolf::Point origin(b.pos.getX(), b.pos.getY());
			float dirX = b.vel.getX();
			float dirY = b.vel.getY();

			Uint8 hitId;
			if (_wolf->checkCollision(origin, dirX, dirY, b.shooter, hitId)) {
				b.used = false;

				Uint32 timestamp = SDL_GetTicks();
				Uint8 buffer[1 + 1 + 1 + 4];
				buffer[0] = _DEAD;
				buffer[1] = hitId;
				buffer[2] = b.shooter;
				memcpy(buffer + 3, &timestamp, 4);
				NET_WriteToStreamSocket(_socket, buffer, sizeof(buffer));
			}
		}
	}
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
			if (ihdlr.isKeyDown(SDL_SCANCODE_R) && _clientId == _masterId) {
				Uint8 buffer[1] = { _RESTART };
				NET_WriteToStreamSocket(_socket, buffer, 1);
			}
		}

		// Recibir mensajes de red
		receiveMessages();

		// Detectar inputs locales
		_wolf->update();

		// Enviar estado al servidor
		sendPlayerState();
		checkAndSendShoot();

		// Actualizar balas
		_bm->update();

		// Verificar colisiones (solo master)
		check_collisions();

		// Renderizar
		sdlutils().clearRenderer();
		_wolf->render();
		_bm->render();
		sdlutils().presentRenderer();

		Uint32 frameTime = vt.currRealTime() - startTime;
		if (frameTime < 16) SDL_Delay(16 - frameTime);
	}
}