// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/System.h"
#include <vector>

struct Transform;

class PacManSystem: public ecs::System {
public:

	PacManSystem();
	virtual ~PacManSystem();
	void initSystem() override;
	void activateShield();
	void deactivateShield();
	void update() override;
	void recieve(const Message&) override;	
private:
	Transform *_pmTR;
	std::vector<Transform*> _shieldTR;

};

