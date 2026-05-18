#pragma once

#include "../ecs/System.h"


class ShieldSystem : public ecs::System
{
	void update() override;
	bool getShield() const { return active; }
	void recieve(const Message& msg) override;

private:
	void activateShield();
	void deactivateShield();
	void resetShield();
	bool active = false;
	float startTime = 0.0f;
};

#pragma once





