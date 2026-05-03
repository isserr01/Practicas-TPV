#pragma once
#include <SDL_rect.h>

#include "GameState.h"

class Texture;

class PausedState : public GameState {
public:
	PausedState() {};
	virtual ~PausedState() {};
	void leave() override;
	void update() override;
	void enter() override;
	void receive(const Message& m) override {};
};



