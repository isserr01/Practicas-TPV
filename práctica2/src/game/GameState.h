// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "messages_defs.h"

class GameState {
public:
	GameState(){}
	virtual ~GameState() {
	}
	virtual void update() = 0;
	virtual void enter() = 0;
	virtual void leave() = 0;
};

