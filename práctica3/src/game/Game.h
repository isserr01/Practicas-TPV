// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <SDL_stdinc.h>
#include <vector>

#include "../utils/Singleton.h"

class LittleWolf;  // ← Cambiar Fighter por LittleWolf
class Bullets;
class Networking;

class Game : public Singleton<Game> {
	friend Singleton<Game>;

public:
	bool init_game(const char* host, Uint16 port, const char* playerName);
	void start();

	LittleWolf& get_littlewolf() {  // ← Cambiar get_fighters por get_littlewolf
		return *_wolf;
	}

	Bullets& get_bullets() {
		return *_bm;
	}

	inline Networking& get_networking() {
		return *_net;
	}

	void sendPlayerState();
	void checkAndSendShoot();

private:
	Game();
	virtual ~Game();
	bool init();

	void check_collisions();

	Bullets* _bm;
	LittleWolf* _wolf;  // ← Cambiar _fighters por _wolf
	Networking* _net;
};