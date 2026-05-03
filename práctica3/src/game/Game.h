// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <SDL_stdinc.h>
#include <vector>

#include "../utils/Singleton.h"

class LittleWolf;
class Bullets;
class Networking;

class Game : public Singleton<Game> {
	friend Singleton<Game>;

public:
	bool init_game(const char* host, Uint16 port, const char* playerName);
	void start();

	LittleWolf& get_littlewolf() {
		return *_wolf;
	}

	Bullets& get_bullets() {
		return *_bm;
	}

	inline Networking& get_networking() {
		return *_net;
	}

private:
	Game();
	virtual ~Game();
	bool init();

	void check_collisions();
	void sendPlayerState();
	void checkAndSendShoot();

	Bullets* _bm;
	LittleWolf* _wolf;
	Networking* _net;

	Uint8 _clientId;
	Uint8 _masterId;
	bool _running;
	Uint32 _lastStateSend;
	float _lastSentX, _lastSentY, _lastSentTheta;
	std::string _playerName;
};