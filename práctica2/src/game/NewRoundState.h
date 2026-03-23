#pragma once
#include <SDL_rect.h>

#include "GameState.h"
#include <iostream>

class Texture;

class NewRoundState : public GameState {
public:
	NewRoundState() {};
	virtual ~NewRoundState() {};
	void leave() override {};
	void update() override;
	void enter() override {};

};

