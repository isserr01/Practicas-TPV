#pragma once

#include "../ecs/System.h"
#include "../game/messages_defs.h"
#include "../ecs/Entity.h"
#include "../ecs/EntityManager.h"

struct Transform;
struct Entity;

const int N = 2;
const float T = 5000.0f;
const float V = 2.0f;

class HelpersSystem : public ecs::System
{
public:

	HelpersSystem();
	virtual ~HelpersSystem() {};
	void spawnHelper();
	void update() override;
	void recieve(const Message& msg) override;
	void colissionGhost(ecs::entity_t e);
	void despawnHelper(ecs::entity_t e);

private:
	Transform* hlTR;
	int timerHelper;
	int cantidadHelpers;

};



