// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <SDL_stdinc.h>
#include <SDL3_net/SDL_net.h>
#include <string>
#include <vector>

#include "../utils/Singleton.h"

class LittleWolf;
class Bullets;

class Game : public Singleton<Game> {
	friend Singleton<Game>;

public:
	bool init_game(const char* host, Uint16 port, const char* playerName);
	void start();

	LittleWolf& get_littlewolf() { return *_wolf; }
	Bullets& get_bullets() { return *_bm; }

	void sendPlayerState();
	void checkAndSendShoot();

private:
	Game();
	virtual ~Game();

	bool init();
	void check_collisions();
	void receiveMessages();

	// Métodos de red
	bool connectToServer(const char* host, Uint16 port);
	void handleServerMessage(const Uint8* data, size_t size);

	void handleNewClient(Uint8 id, Uint8 masterId);
	void handleClientDisconnected(Uint8 id, Uint8 masterId);
	void handlePlayerState(Uint8 clientId, const Uint8* data);
	void handlePlayerInfo(Uint8 clientId, const Uint8* data);
	void handleShoot(Uint8 clientId, const Uint8* data);
	void handleDead(Uint8 clientId, const Uint8* data);
	void handleRestart();

	Bullets* _bm;
	LittleWolf* _wolf;

	// Red
	NET_StreamSocket* _socket;
	Uint8 _clientId;
	Uint8 _masterId;
	bool _running;

	// Control de envío de estado
	Uint32 _lastStateSend;
	float _lastSentX, _lastSentY, _lastSentTheta;
	std::string _playerName;
};