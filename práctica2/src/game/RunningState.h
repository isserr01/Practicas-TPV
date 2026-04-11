#pragma once
#include <SDL_rect.h>

#include "GameState.h"
#include <iostream>
#include <vector>
#include "../ecs/EntityManager.h"


class Texture;

class RunningState : public GameState {
public:
	RunningState() {};
	virtual ~RunningState() {};
	void leave() override {};
	void update() override;
	void enter() override;
	void receive(const Message& m) override;
private:
	ecs::EntityManager* _gm;
	Transform* _fighter;
	float _lastAsteroidTime = 0;

	//systems
	ecs::System* _pacmanSys;
	ecs::System* _gameCtrlSys;
	ecs::System* _ghostSys;
	ecs::System* _starsSys;
	ecs::System* _renderSys;
	ecs::System* _collisionSys;

	std::vector<ecs::System*> systems;

};

