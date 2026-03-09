// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>

#include "../ecs/ecs.h"

class Game {
public:
	Game();
	virtual ~Game();
	void init();
	void start();
private:
	ecs::EntityManager *_mngr;
	ecs::System *_pacmanSys;
	ecs::System *_gameCtrlSys;
	ecs::System *_startsSys;
	ecs::System *_renderSys;
	ecs::System *_collisionSys;
};

