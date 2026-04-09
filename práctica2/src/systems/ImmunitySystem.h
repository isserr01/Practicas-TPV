#pragma once

#include "../ecs/System.h"

class ImmunitySystem : public ecs::System {
public:
	ImmunitySystem();
	virtual ~ImmunitySystem();
	void initSystem() override;
	void update() override;

};



