#pragma once
#include <vector>
#include "../ecs/System.h"
#include "../game/messages_defs.h"
#include "../ecs/Entity.h"
#include "../ecs/EntityManager.h"

struct Transform;
struct Entity;

class GhostSystem : public ecs::System {
public:
	GhostSystem();
	virtual ~GhostSystem();
	void spawnGhost();
	void update() override;
private:
	std::vector<ecs::Entity*> ghosts;
	Transform* gmTR;
	int timerGhost;
	bool immunity;
	int cantidadGhosts;
};

