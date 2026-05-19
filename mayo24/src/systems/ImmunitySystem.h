#pragma once

#include "../ecs/System.h"

class ImmunitySystem : public ecs::System {
public:
	void update() override;
	bool getImmunity() const { return active; }
	void recieve(const Message& msg) override;	
private:
	void activateImmunity();
	void deactivateImmunity();
	void resetImmunity();
	bool active=false;
	float startTime=0.0f;
};



