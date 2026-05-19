#pragma once

#include "../ecs/System.h"

class SafeSystem : public ecs::System
{
public:
	void update() override;
	void recieve(const Message& msg);

private:
	void activateSafe();
	void deactivateSafe();
	void resetSafe();

	bool active = false;
	float startTime = 0.0f;
};




