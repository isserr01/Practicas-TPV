#pragma once

#include "../ecs/System.h"

class ImmunitySystem : public ecs::System {
public:
	void update() override;
	bool getImmunity() const { return active; }
	void recieve(const Message& msg);	
private:
	void onFruitEaten(ecs::entity_t fruit);
	void activateImmunity();
	void deactivateImmunity();
	void resetImmunity();
	bool active=false;
	float startTime=0.0f;
};



