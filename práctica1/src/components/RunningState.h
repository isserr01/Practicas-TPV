#pragma once
#include <SDL_rect.h>

#include "GameState.h"
#include <iostream>
#include "../ecs/EntityManager.h"
#include "Transform.h"


class Texture;

class RunningState : public GameState {
public:
	RunningState() {};
	virtual ~RunningState() {};
	void leave() override {};
	void update() override;
	void enter() override { std::cout << "running state"; };

private:
	ecs::EntityManager* _gm;
	Transform* _fighter;
	float _lastAsteroidTime = 0;
};

