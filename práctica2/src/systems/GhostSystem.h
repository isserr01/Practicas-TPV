/*#pragma once

#include "../ecs/System.h"

struct Transform;

class GhostSystem : public ecs::System {
public:

	GhostSystem();
	virtual ~GhostSystem();
	void spawnGhost();
	void update() override;
private:
	Transform* gmTR;
	int timerGhost;
	bool immunity;
	int cantidadGhosts;
};*/

// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/System.h"

struct Transform;

class GhostSystem : public ecs::System {
public:

	GhostSystem();
	virtual ~GhostSystem();
	void initSystem() override;
	void update() override;
private:
	Transform* _pmTR;
};

